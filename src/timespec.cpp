#include <iostream>

#ifndef time_h_INCLUDED
#define time_h_INCLUDED

#ifndef string_h_INCLUDED
#define string_h_INCLUDED

#ifndef errno_h_INCLUDED
#define errno_h_INCLUDED

#ifndef ctime_h_INCLUDED
#define ctime_h_INCLUDED

double timespec_to_ms(const timespec& time_ts) // Copier une adresse prend moins de place que de copier la structure, dans les paramètres. On met const pour préciser qu'on ne passe la valeur qu'en lecture, on ne veut pas la modifier.
{ 
	return time_ts.tv_sec*1e3+time_ts.tv_nsec*1e-6;
}

timespec timespec_from_ms(double time_ms)
{
	struct timespec converted_time;
	converted_time.tv_sec = int(time_ms*1e-3);
	converted_time.tv_nsec = (time_ms*1e-3-int(time_ms*1e-3))*1e9;
	return converted_time;
}

timespec timespec_now()
{
	struct timespec abstime;
	clock_gettime(CLOCK_REALTIME, &abstime);
	std::cout << "secondes: " << abstime.tv_sec << std::endl; 
	std::cout << "nanosecondes: " << abstime.tv_nsec << std::endl;
	return abstime; 
}

timespec timespec_negate(const timespec& time_ts) /* Les nanosecondes ne peuvent pas être négatives. Regarder l'impact sur les autres fonctions */
{
	struct timespec negate_time;
	negate_time.tv_sec = -time_ts.tv_sec;
	negate_time.tv_nsec = time_ts.tv_nsec;
	return negate_time;
}

timespec timespec_normalize(const timespec& time_ts)
{
	struct timespec normalized_time;
	if (time_ts.tv_nsec >= 0)
	{
		normalized_time.tv_sec = int(time_ts.tv_sec) + int(time_ts.tv_nsec/1e9);
		normalized_time.tv_nsec = time_ts.tv_nsec - int(time_ts.tv_nsec/1e9)*1e9;
	}
	else
	{
		normalized_time.tv_sec = int(time_ts.tv_sec) + int(time_ts.tv_nsec/1e9);
		normalized_time.tv_nsec = time_ts.tv_nsec - int(time_ts.tv_nsec/1e9)*1e9;
		normalized_time.tv_nsec = -normalized_time.tv_nsec;
	}
	return normalized_time;
}

timespec timespec_add(const timespec& time1_ts, const timespec& time2_ts)
{
	struct timespec total_time;
	total_time.tv_sec = time1_ts.tv_sec + time2_ts.tv_sec;
	total_time.tv_nsec = time1_ts.tv_nsec + time2_ts.tv_nsec;
	return timespec_normalize(total_time);
}

timespec timespec_subtract(const timespec& time1_ts, const timespec& time2_ts)
{
    struct timespec total_time;
    struct timespec negate_time2;
    negate_time2 = timespec_negate(time2_ts);
    total_time.tv_sec = time1_ts.tv_sec + negate_time2.tv_sec;
    total_time.tv_nsec = time1_ts.tv_nsec - negate_time2.tv_nsec;

    if (total_time.tv_nsec < 0)
    {
        total_time.tv_sec -= 1;
        total_time.tv_nsec += 1e9;
    }

    return timespec_normalize(total_time);
}

timespec timespec_wait(const timespec& delay_ts)
{
	struct timespec rem = {0,0};
	nanosleep(&delay_ts, &rem);
	if (errno == EINTR)
	{
		return timespec_wait(rem);
	}
	return rem;
}

void timespec_print(const timespec& time_ts)
{
	std::cout << "secondes: " << time_ts.tv_sec << std::endl; 
	std::cout << "nanosecondes: " << time_ts.tv_nsec << std::endl;
}

timespec  operator- (const timespec& time_ts)
{
	timespec time_ts_negated;
	time_ts_negated = timespec_negate(time_ts);
	return time_ts_negated;
}

timespec  operator+ (const timespec& time1_ts, const timespec& time2_ts)
{
	struct timespec total_time;
	total_time = timespec_add(time1_ts, time2_ts);
	return total_time;
}

timespec  operator- (const timespec& time1_ts, const timespec& time2_ts)
{
	struct timespec total_time;
	total_time = timespec_subtract(time1_ts, time2_ts);
	return total_time;
}

timespec& operator+= (timespec& time_ts, const timespec& delay_ts)
{
	time_ts = timespec_add(time_ts, delay_ts);
	return time_ts;
}

timespec& operator-= (timespec& time_ts, const timespec& delay_ts)
{
	time_ts = timespec_subtract(time_ts, delay_ts);
	return time_ts;
}

bool operator== (const timespec& time1_ts, const timespec& time2_ts)
{
	if (time1_ts.tv_sec == time2_ts.tv_sec and time1_ts.tv_nsec == time2_ts.tv_nsec)
	{
		return true;
	}

	return false;
}

bool operator!= (const timespec& time1_ts, const timespec& time2_ts)
{
	if (time1_ts.tv_sec == time2_ts.tv_sec and time1_ts.tv_nsec == time2_ts.tv_nsec)
	{
		return false;
	}

	return true;
}

bool operator< (const timespec& time1_ts, const timespec& time2_ts)
{
	if (timespec_to_ms(time1_ts)<timespec_to_ms(time2_ts))
	{
		return true;
	}

	return false;
}

bool operator> (const timespec& time1_ts, const timespec& time2_ts)
{
	if (timespec_to_ms(time1_ts)<timespec_to_ms(time2_ts))
	{
		return false;
	}

	return true;
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		std::cerr << "Usage: " << argv[0] << " <time_in_ms>" << std::endl;
        	return 1;
	}

	double time_ms = std::stod(argv[1]);
	double time_ms_2 = std::stod(argv[2]);

	timespec nouveau_temps = timespec_from_ms(time_ms);
	timespec nouveau_temps_2 = timespec_from_ms(time_ms_2);

	timespec_print(nouveau_temps);
	double time_converted = timespec_to_ms(nouveau_temps);
	std::cout << "reconversion : " << time_converted << " ms" << std::endl;
	std::cout << "temps actuel : " << std::endl;
	timespec_now();

	struct timespec negate_time;
	negate_time = timespec_negate(nouveau_temps);
	std::cout << "Temps opposé : " << std::endl;
	timespec_print(negate_time);

	std::cout << "Opérateur [-] : " << std::endl;
	timespec time_ts_negated;
	time_ts_negated = -nouveau_temps;
	timespec_print(time_ts_negated);

	timespec temps_errone;
	temps_errone.tv_sec = 6;
	temps_errone.tv_nsec = 5678000000;
	timespec temps_corrige;
	temps_corrige = timespec_normalize(temps_errone);
	std::cout << "Temps corrigé : " << std::endl;
	timespec_print(temps_corrige);
	std::cout << "Bonne réponse : 11 s 678 ns " << std::endl;

	timespec temps_errone_2;
	temps_errone_2.tv_sec = 6;
	temps_errone_2.tv_nsec = -5678000000;
	timespec temps_corrige_2;
	temps_corrige_2 = timespec_normalize(temps_errone_2);
	std::cout << "Temps corrigé : " << std::endl;
	timespec_print(temps_corrige_2);
	std::cout << "Bonne réponse : 1 s 678 ns " << std::endl;

	timespec addition;
	addition = timespec_add(nouveau_temps, nouveau_temps_2);
	std::cout << "Addition : " << std::endl;
	timespec_print(addition);

	std::cout << "Opérateur [+] : " << std::endl;
	timespec added_times;
	added_times = nouveau_temps + nouveau_temps_2;
	timespec_print(added_times);

	timespec soustraction;
	soustraction = timespec_subtract(nouveau_temps, nouveau_temps_2);
	std::cout << "Soustraction : " << std::endl;
	timespec_print(soustraction);

	std::cout << "Opérateur [-] (soustraction) : " << std::endl;
	timespec substracted_times;
	substracted_times = nouveau_temps - nouveau_temps_2;
	timespec_print(substracted_times);

	std::cout << "Attente : " << std::endl;
	timespec delay_ts;
	delay_ts.tv_sec = 2;
	delay_ts.tv_nsec = 18171615;
	timespec_wait(delay_ts);
	std::cout << "Fin de l'attente" << std::endl;
	std::cout << "Délai correct : 2 s 18171615 ns " << std::endl;

	std::cout << "Opérateur [+=] : " << std::endl;
	timespec incremented_time;
	incremented_time.tv_sec = 2;
	incremented_time.tv_nsec = 0;
	incremented_time += delay_ts;
	timespec_print(incremented_time); 
	std::cout << "Bonne réponse : 4 s 18171615 ns " << std::endl;  

	std::cout << "Opérateur [-=] : " << std::endl;
	timespec depreciated_time;
	depreciated_time.tv_sec = 4;
	depreciated_time.tv_nsec = 18171615;
	depreciated_time -= delay_ts;
	timespec_print(depreciated_time);  
	std::cout << "Bonne réponse : 2 s 0 ns " << std::endl;  

	std::cout << "Opérateur [==] : " << (nouveau_temps == nouveau_temps_2) << std::endl;
	std::cout << "Opérateur [!=] : " << (nouveau_temps != nouveau_temps_2) << std::endl;
	std::cout << "Opérateur [<] : " << (nouveau_temps < nouveau_temps_2) << std::endl;
	std::cout << "Opérateur [>] : " << (nouveau_temps > nouveau_temps_2) << std::endl;

	return 0;
}

#endif
#endif
#endif
#endif

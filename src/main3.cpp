#include <iostream>
#include <string>
#include <pthread.h>
#include <vector>
#include "timespec.h"

struct Data
{
	unsigned long nLoops;
	double counter;
};

void incr(unsigned int nLoops, double* pCounter)
{
	for (unsigned int i = 0 ; i < nLoops; i++)
	{
		*pCounter+=1.0;
	}
}

void* call_incr(void* v_data) /* void* veut dire qu'il s'agit d'une adresse pour laquelle on ne sait pas vers quoi elle pointe. On peut mettre toutes les variables qu'on veut dans une structure, et passer l'adresse de cette structure en argument  */
{
	Data* p_data = (Data*) v_data;
	incr(p_data->nLoops, &p_data->counter);
	return v_data;
}

int main(int argc, char* argv[])
{
	if (argc != 3)
	{
		std::cerr << "Deux arguments" << std::endl;
        return 1;
	}

	Data data = {std::stoul(argv[1]), 0.0};

	unsigned long nTasks = std::stoul(argv[2]);

	std::vector<pthread_t> incrementThread(nTasks); /* Depuis C99, il n'est plus permis de faire incrementThread[nTasks] */

	timespec start_time = timespec_now();

	for (auto& thid : incrementThread) /* Remplace un for(int i = 0; i < nTasks, i++). Evite de definir le i dont on n'a pas besoin. */
	{
		pthread_create(&thid, nullptr, call_incr, &data);
	}

	for (auto thid : incrementThread) 
	{
		pthread_join(thid, nullptr);
	}
	std::cout << "Counter value = " << data.counter << std::endl;

	timespec end_time = timespec_now();

	timespec substracted_times;
	substracted_times = end_time - start_time;
	timespec_print(substracted_times);

	return 0;
}

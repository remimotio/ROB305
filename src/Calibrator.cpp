#include "Calibrator.h"
#include <cmath>
#include <iostream>
#include <unistd.h>

#include "timespec.hpp"

Calibrator::Calibrator(double samplingPeriod_ms, unsigned nSamples) : Timer(), a(0.0), b(0.0), samples() // Timer() appelle le constructeur de la classe Timer
{
    for (unsigned i = 0; i < nSamples; ++i) // Boucle de calibration. Mesure le temps pris par les boucles et calcule le nombre de boucles par unité de temps. 
    {
	std::cout << "i : " << i << std::endl;
        Looper looper;
        double startTime = timespec_to_ms(timespec_now());
 	double nboucles = looper.runLoop();
	std::cout << "nboucles : " << nboucles << std::endl;
        double endTime = timespec_to_ms(timespec_now());    

        double duree = endTime - startTime;
        samples.push_back(nboucles / duree); // Nombre de boucles par unité de temps
	std::cout << "Nb boucles / unité tps = " << nboucles / duree << std::endl;
    }

    // Régression linéaire pour déterminer les paramètres de calibration a et b. Méthode des moindres carrés. 
    double sum_t = 0.0, sum_nl = 0.0, sum_tnl = 0.0, sum_t_carres = 0.0;

    for (unsigned i = 0; i < samples.size(); ++i)
    {
        double t = samplingPeriod_ms * i;
        double nl = samples[i];

        sum_t += t;
        sum_nl += nl;
        sum_tnl += t * nl;
        sum_t_carres += t * t;
    }

    a = (nSamples * sum_tnl - sum_t * sum_nl) / (nSamples * sum_t_carres - sum_t * sum_t);
    b = (sum_nl - a * sum_t) / nSamples;

    std::cout << "Résultats de la calibration : a = " << a << ", b = " << b << std::endl;
}

double Calibrator::nLoops(double duration_ms) // On calcule le nombre attendu de boucles correspondant à duration_ms, en utilisant a et b
{
    std::cout << "a : " << a << std::endl;
    std::cout << "duration_ms : " << duration_ms << std::endl;
    std::cout << "b : " << b << std::endl;
    return a * duration_ms + b;
}

void Calibrator::callback()
{
}

Calibrator::Looper::Looper() : doStop(false), iLoop(0.0) // iLoop est le nombre de boucles mesurées
{
}

double Calibrator::Looper::runLoop(double nLoops) // Simulation d'une boucle, incrémente iLoop jusqu'à atteindre nLoops
{
    iLoop = 0.0;
    double maxIterations = 1e+08;

    if (nLoops == DBL_MAX)
    {
	std::cout << "nLoops = DBL_MAX" << std::endl;
    }
    else
    {
    	std::cout << "nLoops = " << nLoops << std::endl;
    }

    while (iLoop < maxIterations and iLoop < nLoops and not doStop)
    {
        iLoop += 1.0;
    }

    if (iLoop == maxIterations)
    {
	return stopLoop();
    }

    return 0.0; // duree_boucles;
}

double Calibrator::Looper::getSample()
{
    return iLoop; // Nombre de boucles mesurées
}

double Calibrator::Looper::stopLoop()
{
    doStop = true;
    return getSample(); // Renvoie iLoop, le nombre de boucles mesurées
}


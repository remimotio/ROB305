#include "Chrono.h"
#include "timespec.h"

#include <iostream>

Chrono::Chrono() 
{
    restart();
}

void Chrono::stop() 
{
    if (isActive()) 
    {
        m_stopTime = timespec_now();
    } 
    else 
    {
        std::cout << "Utiliser restart() pour démarrer le chronomètre.\n";
    }
}

void Chrono::restart() // Il est important de remarquer que deux secondes s'écoulent entre l'initialisation de m_startTime et m_stopTime
{
    m_startTime = timespec_now();
    m_stopTime = m_startTime;
}

bool Chrono::isActive() 
{
    double difference = timespec_to_ms(timespec_subtract(m_stopTime, m_startTime));

    return difference == 2000 ;
}

double Chrono::lap() 
{
    timespec currentTime = timespec_now();

    double duree;

    if (isActive()) 
    {
        duree = timespec_to_ms(timespec_subtract(currentTime, m_startTime)) - 2000;

    } 
    else 
    {
        duree = timespec_to_ms(timespec_subtract(currentTime, m_stopTime)) - 2000;
    }

    return duree;
}

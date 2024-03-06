#ifndef Chrono_h_INCLUDED
#define Chrono_h_INCLUDED

#include "timespec.hpp"

class Chrono
{
protected:
    timespec m_startTime; 
    timespec m_stopTime;

public:
    Chrono();
    void stop();
    void restart();
    bool isActive();
    double lap();
};

#endif

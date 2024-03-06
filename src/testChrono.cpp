#include "Chrono.h"
#include <iostream>

void testChrono() 
{
    std::cout << "Démarrage du chronomètre\n";
    Chrono timer;

    timespec delay;
    delay.tv_sec = 3;
    delay.tv_nsec = 0;
    timespec_wait(delay);

    double duree = timer.lap();
    std::cout << "Durée: " << duree << " ms\n";

    std::cout << "Arret...\n";
    timer.stop();

    delay.tv_sec = 2;
    timespec_wait(delay);

    duree = timer.lap();
    std::cout << "Durée écoulée depuis l'arrêt: " << duree << " ms\n";
}

int main() 
{
    testChrono();

    return 0;
}

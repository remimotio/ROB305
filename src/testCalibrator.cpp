#include "CpuLoop.h"
#include <iostream>

int main()
{
    const double samplingPeriod_ms = 200;
    const unsigned nSamples = 10;
    const double duration_ms = 2000;

    Calibrator calibrator(samplingPeriod_ms, nSamples);
    std::cout << "Calibrator initialisé" << std::endl;

    CpuLoop cpuLoop(calibrator);
    std::cout << "cpuLoop initialisé" << std::endl;

    cpuLoop.runtime(duration_ms);
    std::cout << "runtime terminé" << std::endl;

    std::cout << "Calibration results: a = " << calibrator.getA() << ", b = " << calibrator.getB() << std::endl;

    return 0;
}


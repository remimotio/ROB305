#include "CpuLoop.h"

#include <iostream>

CpuLoop::CpuLoop(Calibrator& calibrator): Calibrator(calibrator)
{
}

void CpuLoop::runtime(double duration_ms)
{
    double loops = nLoops(duration_ms);

    Calibrator::Looper looper;
    looper.runLoop(loops);

    std::cout << "Fin de la fonction runtime" << std::endl;
}

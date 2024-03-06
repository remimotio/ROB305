#ifndef CpuLoop_h_INCLUDED
#define CpuLoop_h_INCLUDED

#include "Calibrator.h"

class CpuLoop: public Calibrator
{
public:
	CpuLoop(Calibrator& calibrator); 
	void runtime(double duration_ms);
};

#endif
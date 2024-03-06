#ifndef Calibrator_h_INCLUDED
#define Calibrator_h_INCLUDED

#include "Timer.h"
#include <vector>
#include <cfloat>

class Calibrator: public Timer
{
public:
	class Looper;

private:
	double a;
	double b;
	std::vector<double> samples;

public:
	Calibrator(double samplingPeriod_ms, unsigned nSamples); 
	double nLoops(double duration_ms);
    double getA() const { return a; }
    double getB() const { return b; }

protected :
	virtual void callback();
};

class Calibrator::Looper
{
private:
	bool doStop; 
	double iLoop;

public:
	Looper();
	double runLoop(double nLoops = DBL_MAX);
	double getSample();
	double stopLoop();
};

#endif

#ifndef CountDown_h_INCLUDED
#define CountDown_h_INCLUDED

#include "Timer.h"

class CountDown: public Timer
{
public:
	CountDown(int initialCount); // constructeur

protected :
	virtual void callback();

private:
	int count;
};

#endif


#ifndef Thread_h_INCLUDED
#define Thread_h_INCLUDED

#include "PosixThread.h"

#include "timespec.h"

class Thread: public PosixThread
{
protected:
	timespec start_time;
	timespec stop_time;

public:
	Thread();
	~Thread();
	void start();

protected :
	virtual void run();

private:
	static void* call_run(void* v_thread);

public:
	static void sleep_ms(double delay_ms);
	double startTime_ms();
	double stopTime_ms();
	double execTime_ms();

};

#endif


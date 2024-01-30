#ifndef Timer_h_INCLUDED
#define Timer_h_INCLUDED

#include <time.h> 
#include <signal.h>

class Timer
{
public:
	Timer(); 
	virtual ~Timer(); 
	void start(double duration_ms, bool isPeriodic = false); 
	void stop();

private:
	timer_t tid;
	struct itimerspec its;

protected:
	virtual void callback() = 0; 

private:
	static void call_callback(int sig, siginfo_t* si, void*); 
};

#endif

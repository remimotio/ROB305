#include "Timer.h" 
#include <cstdlib>
#include <unistd.h>

Timer::Timer()
{
	struct sigaction sa;
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = &Timer::call_callback;
	sigemptyset(&sa.sa_mask);

	sigaction(SIGRTMIN, &sa, nullptr);

	struct sigevent sev;
	sev.sigev_notify = SIGEV_SIGNAL;
	sev.sigev_signo = SIGRTMIN;

	sev.sigev_value.sival_ptr = (Timer*) this;

	timer_create(CLOCK_REALTIME, &sev, &tid);
}

Timer::~Timer()
{
	timer_delete(tid);
}

void Timer::start(double duration_ms, bool isPeriodic)
{
    its.it_value.tv_sec = int(duration_ms) * 1e-3;
    its.it_value.tv_nsec = (duration_ms * 1e-3 - int(duration_ms * 1e-3)) * 1e9;

    if (isPeriodic)
    {
        its.it_interval.tv_sec = its.it_value.tv_sec;
        its.it_interval.tv_nsec = its.it_value.tv_nsec;
    }
    else
    {
        its.it_interval.tv_sec = 0;
        its.it_interval.tv_nsec = 0;
    }

    timer_settime(tid, 0, &its, nullptr);
}

void Timer::stop()
{
	timer_settime(0, 0, 0, nullptr);
}

void Timer::call_callback(int, siginfo_t* si, void*)
{
	Timer* pTimer = static_cast<Timer*>(si->si_value.sival_ptr);
	pTimer->callback();
}

#include "Thread2.h"
#include "timespec.h"

#include <iostream> 
#include <stdexcept>

Thread::Thread() : start_time(), started(false), stop_time()
{
}

Thread::~Thread() 
{
}

bool Thread::start()
{
    if (started)
    {
        std::cerr << "Thread déjà démarré" << std::endl;
        return false;
    }

    started = true;
    
    start_time = timespec_now();
    PosixThread::start(call_run, this);
    return true; 
}

void Thread::run()
{
}

double Thread::startTime_ms()
{
    return timespec_to_ms(start_time);
}

double Thread::stopTime_ms()
{
    stop_time = timespec_now();
    return timespec_to_ms(stop_time);
}

double Thread::execTime_ms()
{
    if (stop_time.tv_sec == 0 && stop_time.tv_nsec == 0) 
    {
        throw std::runtime_error("Erreur: thread pas encore arrêté.");
    }

    return stopTime_ms() - startTime_ms();
}

void Thread::sleep_ms(double delay_ms)
{
    struct timespec delay_ts = timespec_from_ms(delay_ms);
    timespec_wait(delay_ts);
}

void* Thread::call_run(void* v_thread) 
{
    Thread* pThread = static_cast<Thread*>(v_thread);
    pThread->run();
    return v_thread;
}

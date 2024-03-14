#include "timespec.h"
#include "Thread2.h"
#include "Mutex.h"
#include <vector>
#include <iostream>

#include "Calibrator.h"
#include "CpuLoop.h"

struct Data
{
    double counter;
    Mutex mutex; 

    Data(double cnt, bool isInversionSafe = true) : counter(cnt), mutex(isInversionSafe) {}
};

enum class Task {IncrementCounter, OtherTask};

class MyThread : public Thread
{
public:
    timespec delay_ts;
    double nLoops;

public:
    MyThread(Data& data, Task task) : sharedData(data), task(task) {}
    void run();

private:
    Data& sharedData;
    Task task;
};

void MyThread::run()
{
    for (unsigned int i = 0; i < nLoops; i++)
    {
        {   
            timespec_wait(delay_ts);
            if (task == Task::IncrementCounter)
            {
                Mutex::Lock lock(sharedData.mutex); 
                
                double tempCounter = sharedData.counter;
                tempCounter += 1.0; 
                sleep_ms(100);
                sharedData.counter = tempCounter;
             }
             else
             { 
                sleep_ms(100);
             }
        } 
    }
    timespec_wait(delay_ts);
    Mutex::Lock unlock(sharedData.mutex); 
}

int main()
{
    double counter = 0.0; // Compteur partagé

    // Thread principal pour le temps
    
    const double samplingPeriod_ms_P = 10;
    const unsigned nSamples_P = 15;
    
    Calibrator calibrator_P(samplingPeriod_ms_P, nSamples_P);
    
    Data sharedData_P{counter, true};

    MyThread monThread(sharedData_P, Task::OtherTask);
    
    struct sched_param schedParam;
    schedParam.sched_priority= sched_get_priority_max(SCHED_RR);
    pthread_setschedparam(pthread_self(), SCHED_RR, &schedParam);
    
    int p_policy;
    int p_priority;
    monThread.PosixThread::getScheduling(&p_policy, &p_priority);
    std::cout << "p_policy: " << p_policy << ", p_priority = " << p_priority << std::endl;
    
    // Thread A
    
    const double samplingPeriod_ms_A = 10;
    const unsigned nSamples_A = 4;
    const double duration_ms_A = 40;

    Calibrator calibrator_A(samplingPeriod_ms_A, nSamples_A);

    CpuLoop cpuLoop_A(calibrator_A);

    cpuLoop_A.runtime(duration_ms_A);

    std::cout << "Calibration results A: a = " << calibrator_A.getA() << ", b = " << calibrator_A.getB() << std::endl;
    
    MyThread ThreadA(sharedData_P, Task::IncrementCounter);
    ThreadA.nLoops = static_cast<unsigned long>(calibrator_A.nLoops(duration_ms_A));
    
    ThreadA.start_time = timespec_from_ms(30);
    ThreadA.delay_ts = timespec_from_ms(10);
    ThreadA.PosixThread::setScheduling(p_policy, p_priority-1);
    
    // Thread B
    
    const double samplingPeriod_ms_B = 10;
    const unsigned nSamples_B = 4;
    const double duration_ms_B = 10;

    Calibrator calibrator_B(samplingPeriod_ms_B, nSamples_B);
    
    CpuLoop cpuLoop_B(calibrator_B);
    
    cpuLoop_B.runtime(duration_ms_B);

    std::cout << "Calibration results B: a = " << calibrator_B.getA() << ", b = " << calibrator_B.getB() << std::endl;
    
    MyThread ThreadB(sharedData_P, Task::OtherTask); // B n'accède pas à la ressource
    ThreadB.nLoops = static_cast<unsigned long>(calibrator_B.nLoops(duration_ms_B));
    
    ThreadB.start_time = timespec_from_ms(30);
    ThreadB.delay_ts = timespec_from_ms(10);
    ThreadB.PosixThread::setScheduling(p_policy, p_priority-2);
    
    // Thread C
    
    const double samplingPeriod_ms_C = 10;
    const unsigned nSamples_C = 4;
    const double duration_ms_C = 50;

    Calibrator calibrator_C(samplingPeriod_ms_C, nSamples_C);
    
    CpuLoop cpuLoop_C(calibrator_C);
    
    cpuLoop_C.runtime(duration_ms_C);

    std::cout << "Calibration results C: a = " << calibrator_C.getA() << ", b = " << calibrator_C.getB() << std::endl;
    
    MyThread ThreadC(sharedData_P, Task::IncrementCounter);
    ThreadC.nLoops = static_cast<unsigned long>(calibrator_C.nLoops(duration_ms_C));
    
    ThreadC.start_time = timespec_from_ms(0);
    ThreadC.delay_ts = timespec_from_ms(20);
    ThreadC.PosixThread::setScheduling(p_policy, p_priority-3);
    
    if (!monThread.start()) 
    {
        std::cerr << "Thread principal non démarré" << std::endl;
        return 1;
    }
    
    double starting_time = monThread.startTime_ms();
    std::cout << "Heure de début : " << starting_time << " ms" << std::endl;

    if (!ThreadA.start() || !ThreadB.start() || !ThreadC.start()) 
    {
        std::cerr << "Thread non démarré" << std::endl;
        return 1;
    }

    monThread.join();
    ThreadA.join();
    ThreadB.join();
    ThreadC.join();

    double stopping_time = monThread.stopTime_ms();
    std::cout << "Heure de fin : " << stopping_time << " ms" << std::endl;

    std::cout << "Counter value = " << sharedData_P.counter << std::endl;

    double execution_time = stopping_time - starting_time;

    std::cout << "Temps d'exécution : " << execution_time << " ms" << std::endl;

    return 0;
}


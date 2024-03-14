#include "timespec.h"
#include <vector>
#include "Thread.h"

struct Data
{
    unsigned long nLoops;
    double counter;

    Data() : nLoops(2), counter(1) {}
    Data(unsigned long loops, double& refCounter) : nLoops(loops), counter(refCounter) {}
};

class MyThread : public Thread 
{
public:
    void run();
    void* call_run(void* v_data);

public:
    MyThread(Data& data) : sharedData(data) {}
    MyThread() : sharedData(Data()) {}

private:
    Data sharedData;
};

void MyThread::run()
{
    for (unsigned int i = 0 ; i < sharedData.nLoops; i++)
    {
        double tempCounter = sharedData.counter; 
        tempCounter += 1.0; // Copie locale
        sleep_ms(100);
        sharedData.counter = tempCounter;
	std::cout << "Local counter value = " << tempCounter << std::endl;
    }
    std::cout << "Counter value = " << sharedData.counter << std::endl;
}

void* MyThread::call_run(void* v_thread)
{
    MyThread* p_thread = (MyThread*) v_thread;
    p_thread->run();
    return v_thread;
}

int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        std::cerr << "Deux arguments" << std::endl;
        return 1;
    }

    double counter = 0.0;

    Data sharedData{std::stoul(argv[1]), counter};

    sharedData.nLoops = std::stoul(argv[1]);

    MyThread monThread(sharedData);

    unsigned long nTasks = std::stoul(argv[2]);

    std::vector<MyThread> incrementThread(nTasks);

    double starting_time = monThread.startTime_ms();
    std::cout << "Heure de début : " << starting_time << " ms" << std::endl;

    for (auto& thid : incrementThread) 
    {
        thid.start();
    }

    for (auto& thid : incrementThread) 
    {
        thid.join();
    }

    double stopping_time = monThread.stopTime_ms();
    std::cout << "Heure de fin : " << stopping_time << " ms" << std::endl;

    std::cout << "Counter value = " << sharedData.counter << std::endl;

    double execution_time = stopping_time - starting_time;

    std::cout << "Temps d'exécution : " << execution_time << " ms" << std::endl;

    return 0;
}

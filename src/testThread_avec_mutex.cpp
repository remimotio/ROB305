#include "timespec.h"
#include "Thread2.h"
#include "Mutex.h"
#include <vector>

struct Data
{
    unsigned long nLoops;
    double counter;
    Mutex mutex;

    Data(unsigned long loops, double cnt) : nLoops(loops), counter(cnt), mutex() {}
};


class MyThread : public Thread
{
public:
    void run();

public:
    MyThread(Data& data) : sharedData(data), starting_time(timespec_now()) {}

private:
    Data& sharedData;
    timespec starting_time;
};

void MyThread::run()
{
    for (unsigned int i = 0; i < sharedData.nLoops; i++)
    {
        {
            Mutex::Lock lock(sharedData.mutex); 

            double tempCounter = sharedData.counter;
            tempCounter += 1.0; 
            sleep_ms(100);
            sharedData.counter = tempCounter;
        } 
        std::cout << "Local counter value = " << sharedData.counter << std::endl;
    }
    std::cout << "Counter value = " << sharedData.counter << std::endl;
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
    
    unsigned long nTasks = std::stoul(argv[2])-1;
    
    std::vector<MyThread> incrementThread(nTasks, MyThread(sharedData));
    
    double starting_time = monThread.startTime_ms();
    std::cout << "Heure de début : " << starting_time << " ms" << std::endl;

    if (!monThread.start()) 
    {
        std::cerr << "Thread principal non démarré" << std::endl;
        return 1;
    }
    
    for (auto& thid : incrementThread)
    {
        if (!thid.start()) 
        {
            std::cerr << "Thread non démarré" << std::endl;
            return 1;
        }
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

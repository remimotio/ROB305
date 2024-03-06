#include "Mutex.h"
#include "Thread2.h"
#include "timespec.hpp"
#include <vector>

#include <thread> // Pour std::this_thread
#include <chrono> // Pour std::chrono

const int nCons = 5; // Consumer threads
const int tokensPerProducer = 3; 
const int nProd = 5; // Producer threads

class MyThread : public Thread {
public:
    MyThread() = default;
    MyThread(const MyThread&) = delete; 
    MyThread(MyThread&& other) noexcept; 
    ~MyThread() override;

    void run() override;

private:
    MyThread& operator=(const MyThread&) = delete;
    MyThread& operator=(MyThread&&) = delete; 

private:
    std::thread nativeThread;
};

MyThread::MyThread(MyThread&& other) noexcept : Thread(std::move(other)), nativeThread(std::move(other.nativeThread)) {}

MyThread::~MyThread() {
    if (nativeThread.joinable()) {
        nativeThread.join();
    }
}


Mutex::Semaphore semaphore(0, nCons * tokensPerProducer);

void MyThread::run()
{
    while (true)
    {
        if (!semaphore.take(500)) // Timeout
        {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main() {
    std::vector<MyThread> threads;

    threads.reserve(nProd + nCons); 

    for (int i = 0; i < nProd; ++i) {
        threads.emplace_back();
        threads.back().start();
    }

    for (int i = 0; i < nCons; ++i) {
        threads.emplace_back();
        threads.back().start();
    }

    for (auto& thread : threads) {
        thread.join();
    }

    std::cout << "Tous les jetons ont été produits ou consommés." << std::endl;

    return 0;
}



#include "Mutex.h"

#include <cstring> 
#include <iostream>
#include "timespec.hpp"

Mutex::Mutex()
{
	pthread_mutexattr_init(&mutexAttribute);
	pthread_mutexattr_settype(&mutexAttribute, PTHREAD_MUTEX_RECURSIVE);

	pthread_mutex_init(&posixMutexId, &mutexAttribute);
	pthread_cond_init(&posixCondId, nullptr);
}

Mutex::~Mutex()
{
	pthread_mutex_destroy(&posixMutexId);
}

void Mutex::lock()
{
	pthread_mutex_lock(&posixMutexId);
}

bool Mutex::lock(double timeout_ms)
{
	timespec timeout = timespec_from_ms(timeout_ms);

	int result = pthread_mutex_timedlock(&posixMutexId, &timeout);

    if (result == 0) 
    {
    	return true; 
    } 
    else if (result == ETIMEDOUT) 
    {
    	return false; 
    } 
    else 
    {
    	std::cerr << "Mutex lock error: " << strerror(result) << std::endl;
        return false;
    }
}

/*
bool Mutex::trylock()
{

}
*/

void Mutex::unlock()
{
	pthread_mutex_unlock(&posixMutexId);
}

class Mutex::Monitor::TimeoutException {};

Mutex::Monitor::Monitor(Mutex& mutex) : mutex(mutex) // initialisation
{

}

void Mutex::Monitor::wait()
{
	mutex.lock();
	mutex.unlock();
        pthread_cond_wait(&mutex.posixCondId, &mutex.posixMutexId);
	
}

bool Mutex::Monitor::wait(double timeout_ms)
{
	timespec timeout = timespec_from_ms(timeout_ms);
	mutex.lock();
	mutex.unlock(); 
     
    int result = pthread_cond_timedwait(&mutex.posixCondId, &mutex.posixMutexId, &timeout);

	if (result == 0) 
	{
    	return true;  
    } 
    else if (result == ETIMEDOUT) 
    {
       	throw TimeoutException();
    } 
    else 
    {
    	std::cerr << "Monitor wait error: " << strerror(result) << std::endl;
		return false;
	}
}

void Mutex::Monitor::notify()
{
	pthread_cond_signal(&mutex.posixCondId);
}

void Mutex::Monitor::notifyAll()
{
	pthread_cond_broadcast(&mutex.posixCondId);
}

Mutex::Lock::Lock(Mutex& mutex) : Monitor(mutex) // Quel constucteur de la classe mère Monitor est appelé pour initialiser la partie de Lock qui hérite de Monitor 
{
	mutex.lock();
}

Mutex::Lock::Lock(Mutex& mutex, double timeout_ms) : Monitor(mutex)
{
    if (!Monitor::wait(timeout_ms)) {
        throw Monitor::TimeoutException();
    }
}

Mutex::Lock::~Lock()
{
	Monitor::notifyAll();
	mutex.unlock();
}

Mutex::Semaphore::Semaphore(unsigned initCount, unsigned maxCount) : counter(initCount), maxCount(maxCount)
{
}

void Mutex::Semaphore::give()
{
    mutexInstance.lock();

    if (counter < maxCount)
    {
        ++counter;
        std::cout << "Jeton donné. Compteur: " << counter << std::endl;
    }
    else
    {
        std::cout << "Saturation, jeton non donné" << std::endl;
    }

    mutexInstance.unlock();
}

void Mutex::Semaphore::take()
{
    Monitor monitorInstance(mutexInstance);

    mutexInstance.lock();

    while (counter == 0)
    {
        monitorInstance.wait();
    }

    --counter;
    std::cout << "Jeton récupéré. Compteur: " << counter << std::endl;

    mutexInstance.unlock();
}

bool Mutex::Semaphore::take(double timeout_ms)
{
    Monitor monitorInstance(mutexInstance);

    bool success = monitorInstance.wait(timeout_ms);

    if (success)
    {
        mutexInstance.lock();
        --counter;
        std::cout << "Jeton pris. Compteur: " << counter << std::endl;
        mutexInstance.unlock();
    }
    else
    {
        std::cout << "Timeout. Aucun jeton pris." << std::endl;
    }

    return success;
}



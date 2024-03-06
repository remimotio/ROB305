#ifndef Mutex_h_INCLUDED
#define Mutex_h_INCLUDED

#include <pthread.h>
#include <climits> // for UINT_MAX

class Mutex
{
public:
	class Monitor;
	class Lock;
	class Semaphore;

protected:
	pthread_mutex_t posixMutexId; // ou posixId
	pthread_cond_t posixCondId;
	pthread_mutexattr_t mutexAttribute;

public:
	Mutex();
	~Mutex(); // Pas besoin de le déclarer virtuel, car on ne veut pas faire de classes dérivées spécifiques et de polymorphisme

protected:
	void lock();
	bool lock(double timeout_ms);
	//bool trylock();
	void unlock();
};

class Mutex::Monitor
{
public:
	Monitor(Mutex& mutex);
	class TimeoutException;

protected:
	Mutex& mutex; // Paramètre que Monitor obtient via son constructeur

public:
	void wait();
	bool wait(double timeout_ms);
	void notify();
	void notifyAll();
};

class Mutex::Lock : public Monitor // dérive de Monitor
{
public:
	Lock(Mutex& m);
	Lock(Mutex& m, double timeout_ms);
	~Lock();
};

class Mutex::Semaphore
{
protected:
    unsigned counter;
    unsigned maxCount;
    Mutex mutexInstance;

public:
    Semaphore(unsigned initCount = 0, unsigned maxCount = UINT_MAX);
    void give();
    void take();
    bool take(double timeout_ms);
};


#endif

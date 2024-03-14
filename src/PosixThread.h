#ifndef PosixThread_h_INCLUDED
#define PosixThread_h_INCLUDED

#include <pthread.h>
#include <string>
#include <stdexcept>

class PosixThread
{
public:
    using ThreadFunc = void* (*)(void*); // Le (*) désigne un pointeur de fonction

    class Exception : public std::runtime_error
    {
    public:
        Exception(const std::string& message);
    }; 

private:
    pthread_t posixId;
    pthread_attr_t posixAttr;

protected:
    bool isActive; // La classe Thread a besoin d'y accéder

public:
    PosixThread();
    PosixThread(pthread_t posixId);
    virtual ~PosixThread(); // On ne serait pas obligés de le déclarer virtuel ici. Mais la classe Thread qui en dérivera comportera des fonctions virtuelles.
    void start(ThreadFunc threadFunc, void* threadArg); // Le paramètre threadFunc, est le pointeur d'une fonction qui prend en argument un (void*)
    void join();
    bool join(double timeout_ms);
    bool setScheduling(int policy, int priority);
    bool getScheduling(int* p_policy, int* p_priority); 
};

#endif

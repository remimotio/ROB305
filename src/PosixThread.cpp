#include "PosixThread.h"
#include "timespec.h"

PosixThread::PosixThread() : posixId(0), isActive(false)  // Faire l'initialisation ici, permet de tout initialiser même les constantes. Initialiser dans le bon ordre.
{
	pthread_attr_init(&posixAttr);
}

PosixThread::PosixThread(pthread_t posixId) : posixId(posixId), isActive(true)
{
	pthread_attr_init(&posixAttr);
	int policy;
	sched_param schedp;

	if (pthread_getschedparam(posixId, &policy, &schedp) < 0)
	{
		isActive = false;
		posixId = 0;
	}

	pthread_attr_setschedpolicy(&posixAttr, policy);
	pthread_attr_setschedparam(&posixAttr, &schedp);
}

PosixThread::~PosixThread()
{
    pthread_attr_destroy(&posixAttr);
}

PosixThread::Exception::Exception(const std::string& message) : std::runtime_error(message)
{

}

void PosixThread::start(ThreadFunc threadFunc, void* threadArg)
{
    isActive = true;
    int creation = pthread_create(&posixId, &posixAttr, threadFunc, threadArg);

    if (creation != 0)
    {
        isActive = false;
        throw Exception("Erreur lors de la création du thread");
    }
}

void PosixThread::join()
{
    int synchronisation = pthread_join(posixId, nullptr);

    if (synchronisation != 0)
    {
        throw Exception("Erreur de synchronisation du thread");
    }
}

bool PosixThread::join(double timeout_ms)
{
    struct timespec timeout = timespec_from_ms(timeout_ms);

    int synchronisation = pthread_timedjoin_np(posixId, NULL, &timeout);

    if (synchronisation == 0)
    {
        return true;
    }
    else if (synchronisation == ETIMEDOUT)
    {
        return false;
    }
    else
    {
        throw Exception("Erreur lors de la synchronisation du thread");
        return false;
    }
}

bool PosixThread::setScheduling(int policy, int priority)
{
    if (!isActive)
    {
        return false;
    }

    pthread_attr_setschedpolicy(&posixAttr, policy);

    sched_param param;
    param.sched_priority = priority;
    pthread_attr_setschedparam(&posixAttr, &param);

    return true;
}

bool PosixThread::getScheduling(int* p_policy, int* p_priority)
{
    if (!isActive)
    {
        return false;
    }

    sched_param param;
    int policy;

    pthread_attr_getschedparam(&posixAttr, &param);
    *p_priority = param.sched_priority;

    pthread_attr_getschedpolicy(&posixAttr, &policy);
    *p_policy = policy;

    return true;
}

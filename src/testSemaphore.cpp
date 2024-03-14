#include "Thread.h"
#include "Mutex.h"
#include <vector>

// "Instanciez un sémaphore initialement vide partagé par 2 types de tâches"
Mutex::Semaphore semaphore(0, 5); 

// "Faites tourner nCons tâches consommatrices et nProd tâches productrices"
const int nProd = 5;
const int nCons = 5;

class ProducerTask : public Thread
{
public:
    void run() override
    {
        for (int i = 0; i < 3; ++i) // Produit 3 jetons
        {
            sleep_ms(100); // Fait quelque chose

            semaphore.give(); // Donne le jeton au sémaphore
        }
    }
};

class ConsumerTask : public Thread
{
public:
    void run() override
    {
        for (int i = 0; i < 3; ++i) // Consomme 3 jetons
        {
            sleep_ms(150); // Fait quelque chose

            semaphore.take(); // Prend le jeton au sémaphore
        }
    }
};

int main()
{
    std::vector<Thread*> threads;

    try
    {
        // Création des tâches productrices
        for (int i = 0; i < nProd; ++i)
        {
            threads.push_back(new ProducerTask());
        }

        // Création des tâches consommatrices
        for (int i = 0; i < nCons; ++i)
        {
            threads.push_back(new ConsumerTask());
        }

        for (Thread* thread : threads)
        {
            thread->start();
        }

        for (Thread* thread : threads)
        {
            thread->join();
            delete thread;
        }
        
    }
    catch (const std::exception& e)
    {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
    
    std::cout << "Tous les jetons ont été produits ou consommés." << std::endl;

    return 0;
}


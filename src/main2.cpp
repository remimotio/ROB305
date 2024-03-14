#include <iostream>
#include <string>
#include "timespec.h"

void incr(unsigned int nLoops, double* pCounter)
{
	for (unsigned int i = 0 ; i < nLoops; i++)
	{
		*pCounter+=1.0;
	}
}

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cerr << "Un argument, nLoops" << std::endl;
        return 1;
	}

	struct timespec start_time = timespec_now();

	unsigned int nLoops = std::stol(argv[1]);
	double counter = 0.0;
	incr(nLoops, &counter);
	std::cout << "Compteur = " << counter << std::endl;

	struct timespec end_time = timespec_now();

	timespec substracted_times;
	substracted_times = end_time - start_time;
	timespec_print(substracted_times);

	return 0;
}

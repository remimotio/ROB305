#include "CountDown.h" 
#include <iostream>
#include <iomanip>

CountDown::CountDown(int initialCount) : count(initialCount)
{
}

void CountDown::callback()
{
	if (count>=0)
	{
		std::cout << "Countdown: " << count << std::endl;
		count--;

		if (count<0) 
		{
			stop();
		}
	}
}


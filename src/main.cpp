#include "CountDown.h"
#include <thread>

int main() {
    CountDown decompte(10); 
    decompte.start(1000.0, true); 

    std::this_thread::sleep_for(std::chrono::seconds(30)); 

    return 0;
}


#include "Kitchen.hpp"
#include "Debug.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono_literals;

plazza::Kitchen::Kitchen(double multiplier, int cooksAmount, long long restockDelay) :
    _multiplier(multiplier), _cooksAmount(cooksAmount), _restockDelay(restockDelay), _expiry(5s)
{
    _lastBaked = std::chrono::steady_clock::now();
}

void plazza::Kitchen::run()
{
    DEBUG << "Entering Kitchen loop" << std::endl;

    while (true) {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        if (currentTime - _lastBaked >= _expiry) {
            break;
        }
    }

    DEBUG << "Exiting Kitchen" << std::endl;
    exit(0);
}

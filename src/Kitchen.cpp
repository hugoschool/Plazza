#include "Kitchen.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

using namespace std::chrono_literals;

plazza::Kitchen::Kitchen(double multiplier, int cooksAmount, long long restockDelay, size_t kitchenID, IPCM &ipc) :
    _multiplier(multiplier), _cooksAmount(cooksAmount), _running(true), _kitchenID(kitchenID), _ipc(ipc),
    _expiry(5s),
    _restockDelay(restockDelay)
{
    _lastBaked = std::chrono::steady_clock::now();
    _lastRestock = std::chrono::steady_clock::now();
}

void plazza::Kitchen::run()
{
    DEBUG << "Entering Kitchen loop" << std::endl;

    while (_running) {
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        if (currentTime - _lastBaked >= _expiry) {
            _running = false;
            break;
        }
        if (currentTime - _lastRestock >= _restockDelay) {
            DEBUG << "Restocked ingredients" << std::endl;
            _lastRestock = std::chrono::steady_clock::now();
        }
    }
    std::string msg = std::to_string(static_cast<int>(StatusCode::STOP));
    msg.append(" ");
    msg.append(std::to_string(_kitchenID));
    _ipc.kitchenToReceptionist(_kitchenID, msg);
    DEBUG << "Exiting Kitchen" << std::endl;
    exit(0);
}

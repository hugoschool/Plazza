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
    _restockDelay(restockDelay),
    _stock(),
    _party(_multiplier, _cooksAmount)
{
    _creationTime = std::chrono::steady_clock::now();
    _lastRestock = std::chrono::steady_clock::now();
}

void plazza::Kitchen::run()
{
    DEBUG << "Entering Kitchen loop" << std::endl;

    // TODO: temporary, remove this later when not needed
    _party.add(Pizza(Pizza::Type::Regina, Pizza::Size::XL));
    while (_running) {
        // TODO: handling pizza message here
        std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
        if (currentTime - _party.getLastBaked() >= _expiry) {
            _running = false;
            break;
        }
        if (currentTime - _lastRestock >= _restockDelay) {
            DEBUG << "Restocking ingredients" << std::endl;
            _lastRestock = std::chrono::steady_clock::now();
            _stock.restock();
        }
    }
    std::string msg = std::to_string(static_cast<int>(StatusCode::STOP));
    msg.append(" ");
    msg.append(std::to_string(_kitchenID));
    _ipc.kitchenToReceptionist(_kitchenID, msg);

    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    DEBUG << "Exiting Kitchen after " << std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _creationTime) << std::endl;
    exit(0);
}

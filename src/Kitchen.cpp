#include "Kitchen.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
#include "Pizza.hpp"
#include <chrono>
#include <iostream>
#include <optional>
#include <string>
#include <thread>
#include "Utils.hpp"

using namespace std::chrono_literals;

plazza::Kitchen::Kitchen(double multiplier, int cooksAmount, long long restockDelay, size_t kitchenID, IPCM &ipc) :
    _multiplier(multiplier), _cooksAmount(cooksAmount), _running(true), _kitchenID(kitchenID), _ipc(ipc),
    _expiry(5s),
    _restockDelay(restockDelay),
    _stock(),
    _party(_multiplier, _cooksAmount, _stock, ipc, kitchenID)
{
    _creationTime = std::chrono::steady_clock::now();
    _lastRestock = std::chrono::steady_clock::now();
}

void plazza::Kitchen::run()
{
    DEBUG << "Entering Kitchen loop" << std::endl;

    while (_running) {
        std::optional<std::string> message = _ipc.readReceptionistMessage(_kitchenID);

        if (message.has_value()) {
            std::optional<Pizza> pizza = Pizza::unpack(message.value());

            if (pizza.has_value()) {
                _party.add(pizza.value());
                _ipc.createAndSendMessage(_kitchenID, StatusCode::OK, std::nullopt);
            } else {
                DEBUG << "Unpacking pizza failed" << std::endl;
            }
        }
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
    _ipc.createAndSendMessage(_kitchenID, StatusCode::STOP, std::nullopt);
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    DEBUG << "Exiting Kitchen after " << std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _creationTime) << std::endl;
    exit(0);
}

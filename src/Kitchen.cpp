#include "Kitchen.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
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

void plazza::Kitchen::sendStatus()
{
    size_t pizzas = _party.getCurrentlyCookingAmount();
    std::string msg = std::to_string(static_cast<int>(StatusCode::STATUS));

    msg.append(" ");
    msg.append(std::to_string(_kitchenID));
    for (int i = 0; i < _cooksAmount; i++) {
        if (i < static_cast<int>(pizzas)) {
            msg.append(" cooking");
        } else {
            msg.append(" free");
        }
    }
    std::unordered_map<std::string, unsigned int> ingredients = _stock.getIngredients();
    for (auto ingredient: ingredients) {
        msg.append(" " + ingredient.first);
        msg.append(" ");
        msg.append(std::to_string(ingredient.second));
    }
    _ipc.kitchenToReceptionist(_kitchenID, msg);
}

void plazza::Kitchen::run()
{
    DEBUG << "Entering Kitchen loop" << std::endl;

    while (_running) {
        std::optional<std::string> message = _ipc.readReceptionistMessage(_kitchenID);

        if (message.has_value()) {
            std::unique_ptr<PizzInterface> pizza = PizzAbstract::unpack(message.value());

            if (pizza != nullptr) {
                _party.add(std::move(pizza));
                _ipc.createAndSendMessage(_kitchenID, StatusCode::OK, nullptr);
            } else if (message.value() == "status") {
                sendStatus();
            } else {
                DEBUG << "Couldn't identify the following message : " << message.value() << std::endl;
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
    _ipc.createAndSendMessage(_kitchenID, StatusCode::STOP, nullptr);
    std::chrono::steady_clock::time_point currentTime = std::chrono::steady_clock::now();
    DEBUG << "Exiting Kitchen after " << std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - _creationTime) << std::endl;
    exit(0);
}

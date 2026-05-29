#include "PizzaParty.hpp"
#include "Cook.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
#include "Pizza.hpp"
#include <mutex>
#include <thread>
#include <iostream>

plazza::PizzaParty::PizzaParty(double multiplier, int cooksAmount, Stock &stock, IPCM &ipc, size_t kitchenID) :
    _multiplier(multiplier),
    _cooksAmount(cooksAmount),
    _stock(stock),
    _ipc(ipc),
    _kitchenID(kitchenID),
    _running(true),
    _currentlyCookingAmount(0),
    _threads(),
    _pizzaQueue(),
    _cv(),
    _mutex()
{
    _lastBaked = std::chrono::steady_clock::now();

    _threads.reserve(_cooksAmount);

    for (int i = 0; i < _cooksAmount; i++) {
        _threads.push_back(std::thread(&PizzaParty::execute, this));
    }
}

plazza::PizzaParty::~PizzaParty()
{
    {
        std::unique_lock lock(_mutex);
        _running = true;
    }

    _cv.notify_all();

    for (std::thread &thread : _threads) {
        if (thread.joinable())
            thread.join();
    }
}

void plazza::PizzaParty::execute()
{
    Cook cook(_stock);

    DEBUG << "Initialized cook thread" << std::endl;
    while (true) {
        {
            std::unique_lock lock(_mutex);

            _cv.wait(lock, [this] {
                return !_pizzaQueue.empty() || _running;
            });

            if (_running == false)
                break;

            if (_pizzaQueue.empty())
                continue;
        }

        DEBUG << "New task has been given, cook is going to execute it" << std::endl;

        Pizza pizza = _pizzaQueue.pop();
        {
            std::unique_lock lock(_mutex);

            _currentlyCookingAmount++;
            DEBUG << "Pizzas currently cooking: " << _currentlyCookingAmount << std::endl;
        }
        cook.execute(pizza, _multiplier);
        {
            std::unique_lock lock(_mutex);

            _currentlyCookingAmount--;
            DEBUG << "Pizzas currently cooking: " << _currentlyCookingAmount << std::endl;
        }

        _ipc.createAndSendMessage(_kitchenID, StatusCode::DONE, pizza);
        _lastBaked = std::chrono::steady_clock::now();
    }
    DEBUG << "Left cook thread" << std::endl;
}

void plazza::PizzaParty::add(Pizza pizza)
{
    DEBUG << "Adding a new pizza to the queue" << std::endl;
    _pizzaQueue.push(pizza);
    _cv.notify_one();
}

std::chrono::steady_clock::time_point plazza::PizzaParty::getLastBaked() const
{
    return _lastBaked;
}

std::size_t plazza::PizzaParty::getQueueSize()
{
    return _pizzaQueue.size();
}

int plazza::PizzaParty::getCurrentlyCookingAmount()
{
    std::unique_lock lock(_mutex);

    return _currentlyCookingAmount;
}

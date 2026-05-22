#include "PizzaParty.hpp"
#include "Cook.hpp"
#include "Debug.hpp"
#include <mutex>
#include <thread>
#include <iostream>

plazza::PizzaParty::PizzaParty(double multiplier, int cooksAmount, Stock &stock) :
    _multiplier(multiplier),
    _cooksAmount(cooksAmount),
    _stock(stock),
    _running(true),
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

            DEBUG << "New task has been given, cook is going to execute it" << std::endl;

            Pizza pizza = std::move(_pizzaQueue.front());
            _pizzaQueue.pop();

            cook.execute(pizza, _multiplier);
            _lastBaked = std::chrono::steady_clock::now();
        }
    }
    DEBUG << "Left cook thread" << std::endl;
}

void plazza::PizzaParty::add(Pizza pizza)
{
    {
        std::unique_lock<std::mutex> lock(_mutex);

        DEBUG << "Adding a new pizza to the queue" << std::endl;
        _pizzaQueue.push(pizza);
    }
    _cv.notify_one();
}

std::chrono::steady_clock::time_point plazza::PizzaParty::getLastBaked() const
{
    return _lastBaked;
}

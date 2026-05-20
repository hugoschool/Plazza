#include "Cook.hpp"
#include "Debug.hpp"
#include "Pizza.hpp"
#include <chrono>
#include <thread>
#include <iostream>

void plazza::Cook::execute(plazza::Pizza &pizza, double multiplier)
{
    const std::chrono::milliseconds duration(static_cast<long>(pizza.getBakingTime() * multiplier));

    DEBUG << "Pre-cooking a pizza for " << duration << std::endl;
    std::this_thread::sleep_for(duration);
    DEBUG << "Pizza has been cooked" << std::endl;
    pizza.setCooked(true);
}

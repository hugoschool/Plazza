#include "Stock.hpp"
#include "Debug.hpp"
#include <iostream>
#include <mutex>

plazza::Stock::Stock() :
    _ingredients({
        {"dough", MAX_UNIT_AMOUNT},
        {"tomato", MAX_UNIT_AMOUNT},
        {"gruyere", MAX_UNIT_AMOUNT},
        {"ham", MAX_UNIT_AMOUNT},
        {"mushrooms", MAX_UNIT_AMOUNT},
        {"steak", MAX_UNIT_AMOUNT},
        {"eggplant", MAX_UNIT_AMOUNT},
        {"goat cheese", MAX_UNIT_AMOUNT},
        {"chief love", MAX_UNIT_AMOUNT}
    }),
    _mutex()
{
}

void plazza::Stock::restock()
{
    std::unique_lock lock(_mutex);

    for (auto &[ingredient, element] : _ingredients) {
        restockElement(ingredient, element);
    }
}

void plazza::Stock::consume(std::string ingredient)
{
    std::unique_lock lock(_mutex);

    try {
        unsigned int &element = _ingredients.at(ingredient);

        if (element > 0) {
            DEBUG << "Consuming " << ingredient << ": " << element << " -> " << element - 1 << std::endl;
            element--;
        }
    } catch (std::exception &) {
        DEBUG << "Couldn't consume element " << ingredient << std::endl;
    }
}

void plazza::Stock::restockElement(std::string ingredient, unsigned int &element)
{
    if (element < MAX_UNIT_AMOUNT) {
        DEBUG << "Restocking " << ingredient << ": " << element << " -> " << element + 1 << std::endl;
        element++;
    }
}

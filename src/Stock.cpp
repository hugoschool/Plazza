#include "Stock.hpp"
#include "Debug.hpp"
#include <iostream>

plazza::Stock::Stock() :
    dough(MAX_UNIT_AMOUNT),
    tomato(MAX_UNIT_AMOUNT),
    gruyere(MAX_UNIT_AMOUNT),
    ham(MAX_UNIT_AMOUNT),
    mushrooms(MAX_UNIT_AMOUNT),
    steak(MAX_UNIT_AMOUNT),
    eggplant(MAX_UNIT_AMOUNT),
    goatCheese(MAX_UNIT_AMOUNT),
    chiefLove(MAX_UNIT_AMOUNT)
{
}

void plazza::Stock::restock()
{
    restockElement(dough);
    restockElement(tomato);
    restockElement(gruyere);
    restockElement(ham);
    restockElement(mushrooms);
    restockElement(steak);
    restockElement(eggplant);
    restockElement(goatCheese);
    restockElement(chiefLove);
}

void plazza::Stock::restockElement(unsigned int &element)
{
    if (element < MAX_UNIT_AMOUNT) {
        DEBUG << element << " -> " << element + 1 << std::endl;
        element++;
    }
}

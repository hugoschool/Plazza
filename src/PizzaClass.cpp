#include "PizzaClass.hpp"

plazza::Margarita::Margarita(pizza::Size size) : PizzAbstract(size, pizza::Type::Margarita) {}

std::string plazza::Margarita::getTypeString(void) const
{ return "Margarita"; }

double plazza::Margarita::getBakingTime() const
{ return 1000; }

void plazza::Margarita::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("gruyere");
}


plazza::Regina::Regina(pizza::Size size) : PizzAbstract(size, pizza::Type::Regina) {}

std::string plazza::Regina::getTypeString(void) const
{ return "Regina"; }

double plazza::Regina::getBakingTime() const
{ return 2000; }

void plazza::Regina::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("gruyere");
    stock.consume("ham");
    stock.consume("mushrooms");
}


plazza::Americana::Americana(pizza::Size size) : PizzAbstract(size, pizza::Type::Americana) {}

std::string plazza::Americana::getTypeString(void) const
{ return "Americana"; }

double plazza::Americana::getBakingTime() const
{ return 1000; }

void plazza::Americana::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("gruyere");
    stock.consume("steak");
}


plazza::Fantasia::Fantasia(pizza::Size size) : PizzAbstract(size, pizza::Type::Fantasia) {}

std::string plazza::Fantasia::getTypeString(void) const
{ return "Fantasia"; }

double plazza::Fantasia::getBakingTime() const
{ return 4000; }

void plazza::Fantasia::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("eggplant");
    stock.consume("goatcheese");
    stock.consume("chieflove");
}

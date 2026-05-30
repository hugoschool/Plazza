#include "PizzaClass.hpp"

plazza::Margarita::Margarita(Size size) : PizzAbstract(size) {}

std::string plazza::Margarita::getTypeString(void) const
{ return "Margarita"; }

double plazza::Margarita::getBakingTime() const
{ return 1000; }

std::string plazza::Margarita::pack() const
{ return std::to_string(static_cast<int>(Type::Margarita)) + " " + getSize(_size); }

void plazza::Margarita::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("gruyere");
}


plazza::Regina::Regina(Size size) : PizzAbstract(size) {}

std::string plazza::Regina::getTypeString(void) const
{ return "Regina"; }

double plazza::Regina::getBakingTime() const
{ return 2000; }

std::string plazza::Regina::pack() const
{ return std::to_string(static_cast<int>(Type::Regina)) + " " + getSize(_size); }

void plazza::Regina::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("gruyere");
    stock.consume("ham");
    stock.consume("mushrooms");
}


plazza::Americana::Americana(Size size) : PizzAbstract(size) {}

std::string plazza::Americana::getTypeString(void) const
{ return "Americana"; }

double plazza::Americana::getBakingTime() const
{ return 1000; }

std::string plazza::Americana::pack() const
{ return std::to_string(static_cast<int>(Type::Americana)) + " " + getSize(_size); }

void plazza::Americana::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("gruyere");
    stock.consume("steak");
}


plazza::Fantasia::Fantasia(Size size) : PizzAbstract(size) {}

std::string plazza::Fantasia::getTypeString(void) const
{ return "Fantasia"; }

double plazza::Fantasia::getBakingTime() const
{ return 4000; }

std::string plazza::Fantasia::pack() const
{ return std::to_string(static_cast<int>(Type::Fantasia)) + " " + getSize(_size); }

void plazza::Fantasia::consume(Stock &stock)
{
    stock.consume("dough");
    stock.consume("tomato");
    stock.consume("eggplant");
    stock.consume("goatcheese");
    stock.consume("chieflove");
}

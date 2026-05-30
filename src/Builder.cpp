#include "Builder.hpp"
#include "PizzaClass.hpp"

std::shared_ptr<plazza::PizzInterface> plazza::Builder::buildPizza(PizzAbstract::Type type, PizzAbstract::Size size)
{
    switch(type) {
    case PizzAbstract::Type::Margarita:
        return std::make_shared<Margarita>(size);
    case PizzAbstract::Type::Regina:
        return std::make_shared<Regina>(size);
    case PizzAbstract::Type::Americana:
        return std::make_shared<Americana>(size);
    case PizzAbstract::Type::Fantasia:
        return std::make_shared<Fantasia>(size);
    }
}

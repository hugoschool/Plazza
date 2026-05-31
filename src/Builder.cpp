#include "Builder.hpp"
#include "PizzaClass.hpp"

std::shared_ptr<plazza::PizzInterface> plazza::Builder::buildPizza(pizza::Type type, pizza::Size size)
{
    switch(type) {
    case pizza::Type::Margarita:
        return std::make_shared<Margarita>(size);
    case pizza::Type::Regina:
        return std::make_shared<Regina>(size);
    case pizza::Type::Americana:
        return std::make_shared<Americana>(size);
    case pizza::Type::Fantasia:
        return std::make_shared<Fantasia>(size);
    }
}

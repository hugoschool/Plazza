#include "PizzaClass.hpp"
#include "Debug.hpp"
#include "Utils.hpp"
#include "Builder.hpp"
#include <optional>
#include <iostream>

plazza::PizzAbstract::PizzAbstract(Size size)
    : _cooked(false), _size(size)
{
}

bool plazza::PizzAbstract::getCooked() const
{
    return _cooked;
}

void plazza::PizzAbstract::setCooked(bool state)
{
    DEBUG << "New pizza cooked state: " << std::boolalpha << state << std::endl;
    _cooked = state;
}

std::optional<plazza::PizzAbstract::Type> plazza::PizzAbstract::getType(std::string str)
{
    std::optional optNb = retrieveNb<plazza::PizzAbstract::Type>(str);

    if (optNb.has_value())
        return optNb.value();

    str = Utils::String::tolower(str);
    if (str == "regina")
        return plazza::PizzAbstract::Type::Regina;
    if (str == "margarita")
        return plazza::PizzAbstract::Type::Margarita;
    if (str == "americana")
        return plazza::PizzAbstract::Type::Americana;
    if (str == "fantasia")
        return plazza::PizzAbstract::Type::Fantasia;
    return std::nullopt;
}

std::optional<plazza::PizzAbstract::Size> plazza::PizzAbstract::getSize(std::string str)
{
    std::optional optNb = retrieveNb<plazza::PizzAbstract::Size>(str);

    if (optNb.has_value())
        return optNb.value();

    str = Utils::String::tolower(str);
    if (str == "s")
        return Size::S;
    if (str == "m")
        return Size::M;
    if (str == "l")
        return Size::L;
    if (str == "xl")
        return Size::XL;
    if (str == "xxl")
        return Size::XXL;
    return std::nullopt;
}

std::string plazza::PizzAbstract::getSize(plazza::PizzAbstract::Size size)
{
    return std::to_string(static_cast<int>(size));
}

std::shared_ptr<plazza::PizzInterface> plazza::PizzAbstract::unpack(std::string str)
{
    std::vector<std::string> tokens = Utils::String::split(str, " ");

    if (tokens.size() < 2)
        return nullptr;

    std::optional type = getType(tokens[0]);
    std::optional size = getSize(tokens[1]);
    if (!type.has_value() || !size.has_value())
        return nullptr;

    return Builder::buildPizza(type.value(), size.value());
}

std::shared_ptr<plazza::PizzInterface> plazza::PizzAbstract::unpack(std::smatch matches)
{
    if (matches.size() < 3)
        return nullptr;

    std::optional type = getType(matches[1]);
    std::optional size = getSize(matches[2]);
    if (!type.has_value() || !size.has_value())
        return nullptr;

    return Builder::buildPizza(type.value(), size.value());
}

std::string plazza::PizzAbstract::getSizeString(void) const
{
    switch (_size) {
        case Size::S:
            return "S";
        case Size::M:
            return "M";
        case Size::L:
            return "L";
        case Size::XL:
            return "XL";
        case Size::XXL:
            return "XXL";
    }
    return "";
}

std::ostream &operator<<(std::ostream &s, const std::shared_ptr<plazza::PizzInterface> pizza)
{
    std::string type;
    std::string size;
    s << pizza->getSizeString() << " " << pizza->getTypeString();
    return s;
}

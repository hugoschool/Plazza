#include "Pizza.hpp"
#include "Debug.hpp"
#include "Utils.hpp"
#include <optional>
#include <iostream>

plazza::Pizza::Pizza(Type type, Size size)
    : _type(type), _size(size), _cooked(false)
{
}

bool plazza::Pizza::getCooked() const
{
    return _cooked;
}

void plazza::Pizza::setCooked(bool state)
{
    DEBUG << "New pizza cooked state: " << std::boolalpha << state << std::endl;
    _cooked = state;
}

double plazza::Pizza::getBakingTime() const
{
    switch (_type) {
        case Type::Margarita:
            return 1000;
        case Type::Regina:
            return 2000;
        case Type::Americana:
            return 2000;
        case Type::Fantasia:
            return 4000;
    }
}

std::optional<plazza::Pizza::Type> plazza::Pizza::getType(std::string str)
{
    std::optional optNb = retrieveNb<plazza::Pizza::Type>(str);

    if (optNb.has_value())
        return optNb.value();

    str = Utils::String::tolower(str);
    if (str == "regina")
        return plazza::Pizza::Type::Regina;
    if (str == "margarita")
        return plazza::Pizza::Type::Margarita;
    if (str == "americana")
        return plazza::Pizza::Type::Americana;
    if (str == "fantasia")
        return plazza::Pizza::Type::Fantasia;
    return std::nullopt;
}

std::string plazza::Pizza::getType(plazza::Pizza::Type type)
{
    return std::to_string(static_cast<int>(type));
}

std::optional<plazza::Pizza::Size> plazza::Pizza::getSize(std::string str)
{
    std::optional optNb = retrieveNb<plazza::Pizza::Size>(str);

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

std::string plazza::Pizza::getSize(plazza::Pizza::Size size)
{
    return std::to_string(static_cast<int>(size));
}

std::string plazza::Pizza::pack() const
{
    return getType(_type) + " " + getSize(_size);
}

std::optional<plazza::Pizza> plazza::Pizza::unpack(std::string str)
{
    std::vector<std::string> tokens = Utils::String::split(str, " ");

    if (tokens.size() < 2)
        return std::nullopt;

    std::optional type = getType(tokens[0]);
    std::optional size = getSize(tokens[1]);
    if (!type.has_value() || !size.has_value())
        return std::nullopt;

    return Pizza(type.value(), size.value());
}

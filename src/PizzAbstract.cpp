#include "PizzAbstract.hpp"
#include "Debug.hpp"
#include "Utils.hpp"
#include <optional>
#include <iostream>

plazza::PizzAbstract::PizzAbstract(Type type, Size size)
    : _type(type), _size(size), _cooked(false)
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

double plazza::PizzAbstract::getBakingTime() const
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

std::string plazza::PizzAbstract::getType(plazza::PizzAbstract::Type type)
{
    return std::to_string(static_cast<int>(type));
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

std::string plazza::PizzAbstract::pack() const
{
    return getType(_type) + " " + getSize(_size);
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

    return std::make_shared<PizzAbstract>(type.value(), size.value());
}

std::shared_ptr<plazza::PizzInterface> plazza::PizzAbstract::unpack(std::smatch matches)
{
    if (matches.size() < 3)
        return nullptr;

    std::optional type = getType(matches[1]);
    std::optional size = getSize(matches[2]);
    if (!type.has_value() || !size.has_value())
        return nullptr;

    return std::make_shared<PizzAbstract>(type.value(), size.value());
}

void plazza::PizzAbstract::consume(Stock &stock)
{
    switch (_type) {
        case Type::Regina:
            stock.consume("dough");
            stock.consume("tomato");
            stock.consume("gruyere");
            stock.consume("ham");
            stock.consume("mushrooms");
            break;
        case Type::Margarita:
            stock.consume("dough");
            stock.consume("tomato");
            stock.consume("gruyere");
            break;
        case Type::Americana:
            stock.consume("dough");
            stock.consume("tomato");
            stock.consume("gruyere");
            stock.consume("steak");
            break;
        case Type::Fantasia:
            stock.consume("dough");
            stock.consume("tomato");
            stock.consume("eggplant");
            stock.consume("goatcheese");
            stock.consume("chieflove");
            break;
    }
}

std::string plazza::PizzAbstract::getTypeString(void) const
{
    switch (_type) {
        case Type::Regina:
            return "Regina";
        case Type::Margarita:
            return "Margarita";
        case Type::Fantasia:
            return "Fantasia";
        case Type::Americana:
            return "Americana";
    }
    return "";
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

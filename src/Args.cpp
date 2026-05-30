#include "Args.hpp"
#include "Exception.hpp"
#include <iostream>
#include <string>

plazza::Args::Args(int argc, char **argv) : _argc(argc), _help(false)
{
    _argv.reserve(_argc);
    for (int i = 0; i < _argc; i++) {
        _argv.push_back(argv[i]);
    }
    if (_argc == 2 && _argv[1] == "-h") {
        std::cout << "USAGE: ./plazza [arg1] [arg2] [arg3]" << std::endl;
        std::cout << "  First argument corresponds to the multiplier used for cooking the pizzas." << std::endl;
        std::cout << "  Second argument is the number of cooks per kitchen." << std::endl;
        std::cout << "  Third argument is the time it takes to refill a kitchen." << std::endl;
        _help = true;
        return;
    }

    if (_argc != 4)
        throw Exception("Incorrect args amount, use ./plazza -h.");
}

bool plazza::Args::isHelp() const
{
    return _help;
}

double plazza::Args::getMultiplier() const
{
    return std::stod(_argv[1]);
}

int plazza::Args::getCooks() const
{
    return std::stoi(_argv[2]);
}

long long plazza::Args::getRestockDelay() const
{
    return std::stoll(_argv[3]);
}

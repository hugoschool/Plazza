#include "Args.hpp"
#include "Exception.hpp"
#include <string>

plazza::Args::Args(int argc, char **argv) : _argc(argc)
{
    if (_argc != 4)
        throw Exception("Incorrect args amount");

    _argv.reserve(_argc);
    for (int i = 0; i < _argc; i++) {
        _argv.push_back(argv[i]);
    }
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

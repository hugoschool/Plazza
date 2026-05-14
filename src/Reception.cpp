#include "Reception.hpp"
#include "Args.hpp"
#include "Exception.hpp"
#include <iostream>

plazza::Reception::Reception(plazza::Args &args) :
    _multiplier(args.getMultiplier()),
    _cooks(args.getCooks()),
    _restockDelay(args.getRestockDelay())
{
    if (_multiplier < 0 || _cooks < 0 || _restockDelay < 0)
        throw Exception("Invalid given argument");
}

void plazza::Reception::run()
{
    std::cout << "Nothing to see here for now." << std::endl;
    while (true) {};
}

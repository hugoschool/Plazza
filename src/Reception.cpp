#include "Reception.hpp"
#include "Args.hpp"
#include "Exception.hpp"
#include "Debug.hpp"
#include "Utils.hpp"
#include <iostream>
#include <regex>

plazza::Reception::Reception(plazza::Args &args) :
    _multiplier(args.getMultiplier()),
    _cooks(args.getCooks()),
    _restockDelay(args.getRestockDelay()),
    _lineRegex("(?:\\s?)+([a-zA-Z]+)\\s+(S|M|L|XL|XXL)\\s+(x[1-9][0-9]*)(?:\\s?)+")
{
    if (_multiplier < 0 || _cooks < 0 || _restockDelay < 0)
        throw Exception("Invalid given argument");
}

void plazza::Reception::run()
{
    std::string line;

    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;

        DEBUG << "Line: " << line << std::endl;

        if (line == "quit")
            break;

        std::vector<std::string> tokens = Utils::String::split(line, ";");
        if (tokens.size() == 0) {
            std::cerr << "Invalid line: " << line << std::endl;
            continue;
        }

        for (std::string &token : tokens) {
            std::smatch matches;

            if (!std::regex_match(token, matches, _lineRegex) || matches.size() != 4) {
                std::cerr << "Invalid match: " << token << std::endl;
                continue;
            }

            std::string pizzaType = matches[1];
            std::string pizzaSize = matches[2];
            std::string pizzaAmount = matches[3];

            std::cout << "Pizza: " << pizzaType << " " << pizzaSize << " " << pizzaAmount << std::endl;
        }
    }
}

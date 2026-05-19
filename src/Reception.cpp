#include "Reception.hpp"
#include "Args.hpp"
#include "Exception.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
#include "Utils.hpp"
#include <iostream>
#include <regex>
#include <string>
#include <unistd.h>

plazza::Reception::Reception(plazza::Args &args) :
    _multiplier(args.getMultiplier()),
    _cooks(args.getCooks()),
    _restockDelay(args.getRestockDelay()),
    _nextKitchenID(0),
    _lineRegex("(?:\\s?)+([a-zA-Z]+)\\s+(S|M|L|XL|XXL)\\s+(x[1-9][0-9]*)(?:\\s?)+"),
    _kitchens()
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
            createKitchen();
        }
        // deplacer cette boucle dans un thread pour que le getline ne soit pas bloquant
        for (size_t i = 0; i < _nextKitchenID; i++) {
            std::optional<std::string> message = _ipc.readKitchenMessage(i);

            if (!message.has_value())
                continue;
            _messageQueue.push(message.value());
        }
        for (size_t i = 0; i < _messageQueue.size(); i++) {
            interpretMessage(_messageQueue.front());
            _messageQueue.pop();
        }
    }
}

void plazza::Reception::interpretMessage(std::string msg)
{
    std::vector<std::string> line_vec = Utils::String::split(msg, " ");

    if (line_vec.size() == 0)
        return;

    const int statusCodeInt = std::stoi(line_vec[0]);

    if (statusCodeInt == 0)
        return;

    const StatusCode statusCode = static_cast<StatusCode>(statusCodeInt);

    DEBUG << "Received status code " << statusCodeInt << std::endl;
    switch (statusCode) {
        case StatusCode::OK:
            break;
        case StatusCode::STOP: {
            const int kitchenId = std::stoi(line_vec[1]);
            _ipc.closeKitchen(kitchenId);
            break;
        }
        case StatusCode::DONE:
            break;
        case StatusCode::REDISTRIBUTE:
            break;
    }
}

void plazza::Reception::createKitchen()
{
    DEBUG << "Kitchens amount: " << _kitchens.size() << std::endl;
    DEBUG << "Creating a kitchen" << std::endl;

    // TODO: load balancing
    _ipc.openKitchen();
    Kitchen kitchen(_multiplier, _cooks, _restockDelay, _nextKitchenID, _ipc);
    _nextKitchenID++;
    pid_t pid = fork();

    if (pid == -1)
        throw Exception("Fork failed");
    if (pid == 0) {
        kitchen.run();
    } else {
        _kitchens.push_back(kitchen);
    }
}

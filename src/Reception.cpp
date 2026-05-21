#include "Reception.hpp"
#include "Args.hpp"
#include "Exception.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
#include "Kitchen.hpp"
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
    _openedKitchen(0),
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

            createKitchen();
            // faire du load balancing ici pour definir a quelle kitchen on envoie
            _ipc.sendPizzaToKitchen(matches, 0);
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
            _ipc.closeKitchen(kitchenId, _openedKitchen);
            break;
        }
        case StatusCode::DONE:
            break;
        case StatusCode::REDISTRIBUTE:
            break;
        // TODO: missing wait on kitchen pids (or they get zombied)
    }
}

void plazza::Reception::createKitchen()
{
    DEBUG << "Current kitchen amount: " << _nextKitchenID << std::endl;
    DEBUG << "Creating a kitchen" << std::endl;

    // TODO: load balancing
    _ipc.openKitchen(_nextKitchenID);
    _openedKitchen++;
    pid_t pid = fork();

    if (pid == -1)
        throw Exception("Fork failed");
    if (pid == 0) {
        Kitchen kitchen(_multiplier, _cooks, _restockDelay, _nextKitchenID, _ipc);

        kitchen.run();
    }

    _nextKitchenID++;
}

#include "Reception.hpp"
#include "Args.hpp"
#include "Exception.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
#include "Kitchen.hpp"
#include "Pizza.hpp"
#include "Utils.hpp"
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <regex>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <sys/wait.h>

plazza::Reception::Reception(plazza::Args &args) :
    _multiplier(args.getMultiplier()),
    _cooks(args.getCooks()),
    _restockDelay(args.getRestockDelay()),
    _running(true),
    _nextKitchenID(0),
    _openedKitchen(0),
    _lineRegex("(?:\\s?)+([a-zA-Z]+)\\s+(S|M|L|XL|XXL)\\s+(x[1-9][0-9]*)(?:\\s?)+"),
    _mutex()
{
    if (_multiplier < 0 || _cooks < 0 || _restockDelay < 0)
        throw Exception("Invalid given argument");
}

void plazza::Reception::messageInterpretorFunc()
{
    while (_running) {
        if (_kitchenMap.empty())
            continue;
        for (size_t i = 0; i < _nextKitchenID; i++) {
            try {
                _kitchenMap.at(i);
            } catch (std::out_of_range) {break;}
            std::optional<std::string> message = _ipc.readKitchenMessage(i);

            if (!message.has_value())
                continue;
            _messageQueue.push(message.value());
        }
        for (size_t i = 0; i < _messageQueue.size(); i++) {
            std::string message = _messageQueue.pop();
            interpretMessage(message);
        }
    }
}

void plazza::Reception::askStatus()
{
    for (auto kitchen: _kitchenMap) {
        _ipc.receptionistToKitchen(kitchen.first, "status");
    }
}

void plazza::Reception::printStatus(std::vector<std::string> line_vec)
{
    std::cout << "Kitchen " << std::stoi(line_vec[1]) << " currently has " << _kitchenMap.at(std::stoi(line_vec[1])).pizzaAmount << " pizzas in its queue." << std::endl;
    for (int i = 2; i < _cooks + 2; i++) {
        std::cout << "Cook number " << i - 1;
        if (line_vec[i] == "cooking") {
            std::cout << " is cooking." << std::endl;
        } else {
            std::cout << " is not cooking." << std::endl;
        }
    }
    std::cout << "Its current stock is the following:" << std::endl;
    for (size_t i = 2 + _cooks; i < line_vec.size() - 1; i += 2) {
        std::cout << line_vec[i + 1] << " pieces of " << line_vec[i] << " left." << std::endl;
    }
}

void plazza::Reception::run()
{
    std::string line;
    std::thread messageInterpretor(&plazza::Reception::messageInterpretorFunc, this);

    while (_running) {
        std::cout << "> ";
        if (!std::getline(std::cin, line))
            break;

        DEBUG << "Line: " << line << std::endl;

        if (line == "quit")
            break;
        if (line == "status") {
            askStatus();
            continue;
        }

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

            std::optional pizzaOpt = Pizza::unpack(matches);
            if (!pizzaOpt.has_value()) {
                std::cerr << "Invalid pizza: " << token << std::endl;
                continue;
            }
            Pizza pizza = pizzaOpt.value();

            int pizzaAmount = std::stoi(matches[3].str().substr(1));
            if (pizzaAmount <= 0) {
                std::cerr << "Invalid pizza amount: " << matches[3] << std::endl;
                continue;
            }

            DEBUG << "Current pizza: " << token << std::endl;

            // Load Balancing
            distributePizzas(pizza, pizzaAmount);
        }
    }
    _running = false;
    messageInterpretor.join();
}

void plazza::Reception::distributePizzas(plazza::Pizza pizza, int &pizzanum)
{
    int minID;

    if (_kitchenMap.size() == 0)
        createKitchen();
    while (pizzanum != 0) {
        minID = _kitchenMap.begin()->first;
        for (auto &kitchen: _kitchenMap) {
            if (kitchen.second.pizzaAmount < _kitchenMap.at(minID).pizzaAmount)
                minID = kitchen.first;
        }
        if (_kitchenMap.at(minID).pizzaAmount + 1 > _cooks * 2) {
            createKitchen();
            minID = _nextKitchenID - 1;
        }
        _ipc.sendPizzaToKitchen(pizza, minID);
        _kitchenMap.at(minID).pizzaAmount += 1;
        pizzanum--;
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

            KitchenContent &content = _kitchenMap.at(kitchenId);
            if (waitpid(content.pid, NULL, 0) == -1) {
                std::cerr << "Waiting for pid " << content.pid << " failed." << std::endl;
            }
            _kitchenMap.erase(kitchenId);
            break;
        }
        case StatusCode::DONE: {
            const int kitchenId = std::stoi(line_vec[1]);
            _kitchenMap.at(kitchenId).pizzaAmount -= 1;
            break;
        }
        case StatusCode::REDISTRIBUTE:
            break;
        case StatusCode::STATUS: {
            printStatus(line_vec);
            break;
        }
    }
}

void plazza::Reception::createKitchen()
{
    DEBUG << "Current kitchen amount: " << _nextKitchenID << std::endl;
    DEBUG << "Creating a kitchen" << std::endl;

    _ipc.openKitchen(_nextKitchenID);
    _openedKitchen++;
    pid_t pid = fork();

    if (pid == -1)
        throw Exception("Fork failed");
    if (pid == 0) {
        Kitchen kitchen(_multiplier, _cooks, _restockDelay, _nextKitchenID, _ipc);

        kitchen.run();
        return;
    }
    _kitchenMap.insert({_nextKitchenID, {
        .pizzaAmount = 0,
        .pid = pid,
    }});
    _nextKitchenID++;
}

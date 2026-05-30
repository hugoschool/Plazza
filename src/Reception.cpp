#include "Reception.hpp"
#include "Args.hpp"
#include "Exception.hpp"
#include "Debug.hpp"
#include "IPCM.hpp"
#include "Kitchen.hpp"
#include "Pizza.hpp"
#include "Utils.hpp"
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <mutex>
#include <regex>
#include <string>
#include <thread>
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
    while (true) {
        {
            std::lock_guard lock(_mutex);

            if (_running == false)
                break;
        }
        if (_kitchenMap.empty())
            continue;
        std::vector<std::size_t> ids = _kitchenMap.keys();
        for (std::size_t id: ids) {
            std::optional<std::string> message = _ipc.readKitchenMessage(id);

            if (message.has_value())
                _messageQueue.push(message.value());
        }
        while (!_messageQueue.empty()) {
            std::string message = _messageQueue.pop();
            interpretMessage(message);
        }
        // Wait for 250ms as to not saturate the message queues
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }
}

void plazza::Reception::askStatus()
{
    std::vector<std::size_t> ids = _kitchenMap.keys();
    for (std::size_t id: ids) {
        _ipc.receptionistToKitchen(id, "status");
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

    while (true) {
        {
            std::lock_guard lock(_mutex);

            if (_running == false)
                break;
        }
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
    {
        std::lock_guard lock(_mutex);

        _running = false;
    }
    messageInterpretor.join();
}

void plazza::Reception::distributePizzas(plazza::Pizza pizza, int &pizzanum)
{
    int minID;

    if (_kitchenMap.size() == 0)
        createKitchen();
    while (pizzanum != 0) {
        minID = _kitchenMap.begin()->first;
        std::vector<std::size_t> ids = _kitchenMap.keys();
        for (std::size_t id: ids) {
            if (_kitchenMap.at(id).pizzaAmount < _kitchenMap.at(minID).pizzaAmount)
                minID = id;
        }
        if (_kitchenMap.at(minID).pizzaAmount + 1 > _cooks * 2) {
            createKitchen();
            minID = _nextKitchenID - 1;
        }
        _ipc.sendPizzaToKitchen(pizza, minID);
        _kitchenMap.applyAt<void(plazza::Reception::KitchenContent &)>(minID, [](plazza::Reception::KitchenContent &elem)
        {
            elem.pizzaAmount += 1;
        });
        pizzanum--;
    }
}

void plazza::Reception::interpretMessage(std::string msg)
{
    DEBUG << "Interpreting message: \"" << msg << "\"" << std::endl;
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
            _kitchenMap.applyAt<void(plazza::Reception::KitchenContent &)>(kitchenId, [](plazza::Reception::KitchenContent &elem)
            {
                elem.pizzaAmount -= 1;
            });
            std::optional<Pizza> pizza = Pizza::unpack(line_vec[2] + " " + line_vec[3]);
            if (!pizza.has_value())
                break;
            std::cout << pizza.value() <<  " just finished cooking in kitchen " << kitchenId << "." << std::endl;
            std::cout << "> " << std::flush;
            break;
        }
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
    _kitchenMap.insert(_nextKitchenID, {
        .pizzaAmount = 0,
        .pid = pid,
    });
    _nextKitchenID++;
}

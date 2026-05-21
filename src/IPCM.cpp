#include "IPCM.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include "Pizza.hpp"
#include <cstring>
#include <fcntl.h>
#include <format>
#include <optional>
#include <string>
#include <iostream>
#include <unistd.h>

plazza::IPCM::IPCM()
{}

plazza::IPCM::~IPCM()
{}

void plazza::IPCM::openKitchen(int index)
{
    int kitchenpipefds[2];
    int receptionistpipefds[2];

    if (pipe(kitchenpipefds) == -1)
        throw Exception("Pipe failed");
    if (pipe(receptionistpipefds) == -1)
        throw Exception("Pipe failed");

    // Make reading pipes non blocking
    if (fcntl(kitchenpipefds[0], F_SETFL, O_NONBLOCK) < 0)
        throw Exception("Pipe failed");
    if (fcntl(receptionistpipefds[0], F_SETFL, O_NONBLOCK) < 0)
        throw Exception("Pipe failed");

    _kitchensfds.insert_or_assign(index, std::pair(kitchenpipefds[0], kitchenpipefds[1]));
    _receptionistfds.insert_or_assign(index, std::pair(receptionistpipefds[0], receptionistpipefds[1]));

    DEBUG << "Kitchen FDs: " << _kitchensfds.size() << std::endl;
    DEBUG << "Receptionist FDs: " << _receptionistfds.size() << std::endl;
}

void plazza::IPCM::sendPizzaToKitchen(std::smatch matches, int index)
{
    DEBUG << "Sending " << matches[0] << " of size " << matches[1] << " to kitchen " << index << std::endl;

    std::string msg(matches[0]);
    msg += " ";
    msg += matches[1];

    receptionistToKitchen(index, msg);
}

void plazza::IPCM::closeKitchen(int index, int &openedKitchen)
{
    close(_kitchensfds.at(index).first);
    close(_kitchensfds.at(index).second);
    close(_receptionistfds.at(index).first);
    close(_receptionistfds.at(index).second);
    openedKitchen--;

    _kitchensfds.erase(index);
    _receptionistfds.erase(index);
    DEBUG << "Successfully closed kitchen, there are now " << openedKitchen << " kitchens still opened" << std::endl;
}

void plazza::IPCM::kitchenToReceptionist(int index, const std::string msg)
{
    DEBUG << "Sending message to receptionist from " << index << ": \"" << msg << "\"" << std::endl;
    write(_receptionistfds.at(index).second, msg.c_str(), msg.length());
}

void plazza::IPCM::receptionistToKitchen(int index, const std::string pizzamsg)
{
    DEBUG << "Sending message to kitchen from " << index << ": \"" << pizzamsg << "\"" << std::endl;
    write(_kitchensfds.at(index).second, pizzamsg.c_str(), pizzamsg.length());
}

std::optional<std::string> plazza::IPCM::readKitchenMessage(int index)
{
    char buffer[BUFFER_SIZE];

    std::memset(buffer, '\0', BUFFER_SIZE);
    if (read(_receptionistfds.at(index).first, buffer, BUFFER_SIZE - 1) <= 0) {
        return std::nullopt;
    };
    return std::string(buffer);
}

std::optional<std::string> plazza::IPCM::readReceptionistMessage(int index)
{
    char buffer[BUFFER_SIZE];

    std::memset(buffer, '\0', BUFFER_SIZE);
    if (read(_kitchensfds.at(index).first, buffer, BUFFER_SIZE - 1) <= 0) {
        return std::nullopt;
    };
    return std::string(buffer);
}

std::map<int, std::pair<int, int>> plazza::IPCM::getKitchenFds() const
{
    return _kitchensfds;
}

std::map<int, std::pair<int, int>> plazza::IPCM::getReceptionistFds() const
{
    return _receptionistfds;
}

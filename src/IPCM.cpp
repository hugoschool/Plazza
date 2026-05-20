#include "IPCM.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include <cstring>
#include <fcntl.h>
#include <optional>
#include <string>
#include <iostream>
#include <unistd.h>

plazza::IPCM::IPCM()
{}

plazza::IPCM::~IPCM()
{}

void plazza::IPCM::openKitchen()
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

    _kitchensfds.push_back(std::pair(kitchenpipefds[0], kitchenpipefds[1]));
    _receptionistfds.push_back(std::pair(receptionistpipefds[0], receptionistpipefds[1]));

    DEBUG << "Kitchen FDs: " << _kitchensfds.size() << std::endl;
    DEBUG << "Receptionist FDs: " << _receptionistfds.size() << std::endl;
}

void plazza::IPCM::closeKitchen(int index)
{
    close(_kitchensfds[index].first);
    close(_kitchensfds[index].second);
    close(_receptionistfds[index].first);
    close(_receptionistfds[index].second);

    DEBUG << "Successfully closed kitchen" << std::endl;
}

void plazza::IPCM::kitchenToReceptionist(int index, const std::string msg)
{
    DEBUG << "Sending message to receptionist from " << index << ": \"" << msg << "\"" << std::endl;
    write(_receptionistfds[index].second, msg.c_str(), msg.length());
}

void plazza::IPCM::receptionistToKitchen(int index, const std::string pizzamsg)
{
    DEBUG << "Sending message to kitchen from " << index << ": \"" << pizzamsg << "\"" << std::endl;
    write(_kitchensfds[index].second, pizzamsg.c_str(), pizzamsg.length());
}

std::optional<std::string> plazza::IPCM::readKitchenMessage(int index)
{
    char buffer[BUFFER_SIZE];

    std::memset(buffer, '\0', BUFFER_SIZE);
    if (read(_receptionistfds[index].first, buffer, BUFFER_SIZE - 1) <= 0) {
        return std::nullopt;
    };
    return std::string(buffer);
}

std::vector<std::pair<int, int>> plazza::IPCM::getKitchenFds() const
{
    return _kitchensfds;
}

std::vector<std::pair<int, int>> plazza::IPCM::getReceptionistFds() const
{
    return _receptionistfds;
}

#include "IPCM.hpp"
#include "Exception.hpp"
#include "Utils.hpp"
#include <csignal>
#include <string>
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
    _kitchensfds.push_back(std::pair(kitchenpipefds[0], kitchenpipefds[1]));
    _receptionistfds.push_back(std::pair(receptionistpipefds[0], receptionistpipefds[1]));
}

void plazza::IPCM::closeKitchen(int index)
{
    close(_kitchensfds[index].first);
    close(_kitchensfds[index].second);
    close(_receptionistfds[index].first);
    close(_receptionistfds[index].second);
    _receptionistfds.erase(_receptionistfds.begin() + index);
    _kitchensfds.erase(_kitchensfds.begin() + index);
}

void plazza::IPCM::kitchenToReceptionist(int index, const std::string msg)
{
    write(_receptionistfds[index].second, msg.c_str(), msg.length());
}

void plazza::IPCM::receptionistToKitchen(int index, const std::string pizzamsg)
{
    write(_kitchensfds[index].second, pizzamsg.c_str(), pizzamsg.length());
}

std::string plazza::IPCM::readKitchenMessage(int index)
{
    char buffer[BUFFER_SIZE];

    read(_receptionistfds[index].first, buffer, BUFFER_SIZE);
    return buffer;
}

std::vector<std::pair<int, int>> plazza::IPCM::getKitchenFds() const
{
    return _kitchensfds;
}

std::vector<std::pair<int, int>> plazza::IPCM::getReceptionistFds() const
{
    return _receptionistfds;
}

#include "IPCM.hpp"
#include "Exception.hpp"
#include <csignal>
#include <unistd.h>

plazza::IPCM::IPCM()
{
    int pipefds[2];

    if (pipe(pipefds) == -1) {
        throw Exception("Pipe failed");
    }
    _receptionistfds.first = pipefds[0];
    _receptionistfds.second = pipefds[1];
}

plazza::IPCM::~IPCM()
{}

void plazza::IPCM::openKitchen()
{
    int kitchenpipefds[2];

    if (pipe(kitchenpipefds) == -1)
        throw Exception("Pipe failed");
    _kitchensfds.push_back(std::pair(kitchenpipefds[0], kitchenpipefds[1]));
}

void plazza::IPCM::kitchenToReceptionist(const std::string msg)
{
    write(_receptionistfds.second, msg.c_str(), msg.length());
}

void plazza::IPCM::receptionistToKitchen(int index, const std::string pizzamsg)
{
    write(_kitchensfds[index].second, pizzamsg.c_str(), pizzamsg.length());
}

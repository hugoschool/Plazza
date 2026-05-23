#include "IPCM.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include "Pizza.hpp"
#include <cstring>
#include <fcntl.h>
#include <format>
#include <mqueue.h>
#include <optional>
#include <string>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

plazza::IPCM::IPCM()
{}

plazza::IPCM::~IPCM()
{}

void plazza::IPCM::openKitchen(int index)
{
    struct mq_attr attr;
    attr.mq_msgsize = BUFFER_SIZE;
    attr.mq_curmsgs = 0;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 10;

    std::string kitchenName = "/Kitchen " + std::to_string(index);
    mqd_t kitchenQueue = mq_open(kitchenName.c_str(), O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);
    if (kitchenQueue == static_cast<mqd_t>(-1))
        throw Exception("Failed to create kitchen queue");
    std::string receptionistName = "/Receptionist " + std::to_string(index);
    mqd_t receptionistQueue = mq_open(receptionistName.c_str(), O_CREAT | O_RDWR | O_NONBLOCK, 0644, &attr);
    if (receptionistQueue == static_cast<mqd_t>(-1))
        throw Exception("Failed to create receptionist queue");

    _kitchenQueues.insert_or_assign(index, kitchenQueue);
    _receptionistQueues.insert_or_assign(index, receptionistQueue);

    DEBUG << "Kitchen queue size: " << _kitchenQueues.size() << std::endl;
    DEBUG << "Receptionist queue size: " << _receptionistQueues.size() << std::endl;
}

void plazza::IPCM::sendPizzaToKitchen(plazza::Pizza &pizza, int index)
{
    receptionistToKitchen(index, pizza.pack());
}

void plazza::IPCM::closeKitchen(int index, int &openedKitchen)
{
    std::string kitchenName = "/Kitchen " + std::to_string(index);
    std::string receptionistName = "/Receptionist " + std::to_string(index);

    mq_close(_kitchenQueues.at(index));
    mq_close(_receptionistQueues.at(index));
    mq_unlink(kitchenName.c_str());
    mq_unlink(receptionistName.c_str());
    _kitchenQueues.erase(index);
    _receptionistQueues.erase(index);

    openedKitchen--;
    DEBUG << "Successfully closed kitchen, there are now " << openedKitchen << " kitchens still opened" << std::endl;
}

void plazza::IPCM::createAndSendMessage(int index, plazza::StatusCode code, std::optional<Pizza> pizza)
{
    std::string msg = std::to_string(static_cast<int>(code));

    msg.append(" ");
    msg.append(std::to_string(index));
    if (pizza.has_value()) {
        // append au message pour y ajouter la pizza
        return;
    }
    kitchenToReceptionist(index, msg);
}

void plazza::IPCM::kitchenToReceptionist(int index, const std::string msg)
{
    DEBUG << "Sending message to receptionist from " << index << ": \"" << msg << "\"" << std::endl;
    mq_send(_receptionistQueues.at(index), msg.c_str(), msg.length() + 1, 0);
}

void plazza::IPCM::receptionistToKitchen(int index, const std::string pizzamsg)
{
    DEBUG << "Sending message to kitchen from " << index << ": \"" << pizzamsg << "\"" << std::endl;
    mq_send(_kitchenQueues.at(index), pizzamsg.c_str(), pizzamsg.length() + 1, 0);
}

std::optional<std::string> plazza::IPCM::readKitchenMessage(int index)
{
    char buffer[BUFFER_SIZE];

    std::memset(buffer, '\0', BUFFER_SIZE);
    ssize_t bytes = mq_receive(_receptionistQueues.at(index), buffer, BUFFER_SIZE, nullptr);
    if (bytes == -1)
        return std::nullopt;
    return std::string(buffer);
}

std::optional<std::string> plazza::IPCM::readReceptionistMessage(int index)
{
    char buffer[BUFFER_SIZE];

    std::memset(buffer, '\0', BUFFER_SIZE);
    ssize_t bytes = mq_receive(_kitchenQueues.at(index), buffer, BUFFER_SIZE, nullptr);
    if (bytes == -1)
        return std::nullopt;
    return std::string(buffer);
}

std::map<int, mqd_t> plazza::IPCM::getKitchenQueues() const
{
    return _kitchenQueues;
}

std::map<int, mqd_t> plazza::IPCM::getReceptionistQueues() const
{
    return _receptionistQueues;
}

#include "IPCM.hpp"
#include "Debug.hpp"
#include "Exception.hpp"
#include <cerrno>
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

mqd_t plazza::IPCM::createMessageQueue(std::string name)
{
    struct mq_attr attr;
    attr.mq_msgsize = BUFFER_SIZE;
    attr.mq_curmsgs = 0;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 10;

    mqd_t queue = mq_open(name.c_str(), O_CREAT | O_RDWR | O_NONBLOCK | O_EXCL, 0644, &attr);

    // O_EXCL makes the command fail if the mq already exists, in that case delete the message queue and try again.
    if (queue == static_cast<mqd_t>(-1) && errno == EEXIST) {
        mq_unlink(name.c_str());
        queue = mq_open(name.c_str(), O_CREAT | O_RDWR | O_NONBLOCK | O_EXCL, 0644, &attr);
    }

    if (queue == static_cast<mqd_t>(-1)) {
        throw Exception("Failed to create receptionist queue");
    }
    return queue;
}

void plazza::IPCM::openKitchen(int index)
{
    struct mq_attr attr;
    attr.mq_msgsize = BUFFER_SIZE;
    attr.mq_curmsgs = 0;
    attr.mq_flags = O_NONBLOCK;
    attr.mq_maxmsg = 10;

    std::string kitchenName = "/Kitchen " + std::to_string(index);
    mqd_t kitchenQueue = createMessageQueue(kitchenName);

    std::string receptionistName = "/Receptionist " + std::to_string(index);
    mqd_t receptionistQueue = createMessageQueue(receptionistName);

    _kitchenQueues.insert(index, kitchenQueue);
    _receptionistQueues.insert(index, receptionistQueue);

    DEBUG << "Kitchen queue size: " << _kitchenQueues.size() << std::endl;
    DEBUG << "Receptionist queue size: " << _receptionistQueues.size() << std::endl;
}

void plazza::IPCM::sendPizzaToKitchen(std::shared_ptr<plazza::PizzInterface> pizza, int index)
{
    receptionistToKitchen(index, pizza->pack());
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

void plazza::IPCM::createAndSendMessage(int index, plazza::StatusCode code, std::shared_ptr<PizzInterface> pizza)
{
    std::string msg = std::to_string(static_cast<int>(code));

    msg.append(" ");
    msg.append(std::to_string(index));
    if (pizza != nullptr) {
        msg.append(" ");
        msg.append(pizza->pack());
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

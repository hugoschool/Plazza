#pragma once

#include "Pizza.hpp"
#include <map>
#include <optional>
#include <string>
#include <mqueue.h>

namespace plazza {
    enum class StatusCode {
        OK = 200,
        STOP = 500,
        DONE = 250,
        STATUS = 300,
    };

    class IPCM {
        private:
            std::map<int, mqd_t> _receptionistQueues;
            std::map<int, mqd_t> _kitchenQueues;

            static constexpr std::size_t BUFFER_SIZE = 256;

        public:
            IPCM();
            ~IPCM();

            void kitchenToReceptionist(int index, const std::string msg);
            void receptionistToKitchen(int index, const std::string pizzamsg);
            std::optional<std::string> readKitchenMessage(int index);
            std::optional<std::string> readReceptionistMessage(int index);
            void openKitchen(int index);
            void closeKitchen(int index, int &openedKitchen);
            void sendPizzaToKitchen(Pizza &pizza, int index);
            void createAndSendMessage(int index, StatusCode code);

            std::map<int, mqd_t> getReceptionistQueues() const;
            std::map<int, mqd_t> getKitchenQueues() const;
    };
}

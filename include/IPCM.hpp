#pragma once

#include "Pizza.hpp"
#include <map>
#include <optional>
#include <string>

namespace plazza {
    enum class StatusCode {
        OK = 200,
        STOP = 500,
        DONE = 250,
        REDISTRIBUTE = 400,
    };

    class IPCM {
        private:
            std::map<int, std::pair<int, int>> _receptionistfds;
            std::map<int, std::pair<int, int>> _kitchensfds;

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
            void createAndSendMessage(int index, StatusCode code, std::optional<Pizza> pizza);

            std::map<int, std::pair<int, int>> getReceptionistFds() const;
            std::map<int, std::pair<int, int>> getKitchenFds() const;
    };
}

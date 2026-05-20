#pragma once

#include <optional>
#include <string>
#include <vector>

namespace plazza {
    enum class StatusCode {
        OK = 200,
        STOP = 500,
        DONE = 250,
        REDISTRIBUTE = 400,
    };

    class IPCM {
        private:
            std::vector<std::pair<int, int>> _receptionistfds;
            std::vector<std::pair<int, int>> _kitchensfds;

            static constexpr std::size_t BUFFER_SIZE = 256;

        public:
            IPCM();
            ~IPCM();

            void kitchenToReceptionist(int index, const std::string msg);
            void receptionistToKitchen(int index, const std::string pizzamsg);
            std::optional<std::string> readKitchenMessage(int index);
            void openKitchen();
            void closeKitchen(int index);

            std::vector<std::pair<int, int>> getReceptionistFds() const;
            std::vector<std::pair<int, int>> getKitchenFds() const;
    };
}

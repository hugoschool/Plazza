#pragma once

#include <string>
#include <vector>

#define BUFFER_SIZE 256

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
        public:
            IPCM();
            ~IPCM();

            void kitchenToReceptionist(int index, const std::string msg);
            void receptionistToKitchen(int index, const std::string pizzamsg);
            std::string readKitchenMessage(int index);
            void openKitchen();
            void closeKitchen(int index);

            std::vector<std::pair<int, int>> getReceptionistFds() const;
            std::vector<std::pair<int, int>> getKitchenFds() const;
    };
}

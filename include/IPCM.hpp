#pragma once

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
            std::pair<int, int> _receptionistfds;
            std::vector<std::pair<int, int>> _kitchensfds;
        public:
            IPCM();
            ~IPCM();

            void kitchenToReceptionist(const std::string msg);
            void receptionistToKitchen(int index, const std::string pizzamsg);
            void openKitchen();
    };
}

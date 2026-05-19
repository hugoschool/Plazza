#pragma once

#include "Args.hpp"
#include "Kitchen.hpp"
#include "IPCM.hpp"
#include <queue>
#include <regex>
#include <vector>

namespace plazza {
    class Reception {
        public:
            Reception() = delete;
            Reception(Args &args);
            ~Reception() = default;

            void run();

        private:
            double _multiplier;
            int _cooks;
            long long _restockDelay;
            size_t _nextKitchenID;
            std::regex _lineRegex;
            IPCM _ipc;
            std::queue<std::string> _messageQueue;

            std::vector<plazza::Kitchen> _kitchens;

            void createKitchen();
            void interpretMessage(std::string);
    };
}

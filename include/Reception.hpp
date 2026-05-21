#pragma once

#include "Args.hpp"
#include "Kitchen.hpp"
#include "IPCM.hpp"
#include <queue>
#include <regex>

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
            int _openedKitchen;
            std::regex _lineRegex;
            IPCM _ipc;
            std::queue<std::string> _messageQueue;
            std::map<int, int> _kitchenMap;

            void createKitchen();
            void interpretMessage(std::string);
            void distributePizzas(std::smatch, int &pizzanum);
    };
}

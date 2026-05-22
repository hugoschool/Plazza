#pragma once

#include "Args.hpp"
#include "IPCM.hpp"
#include "Pizza.hpp"
#include <queue>
#include <regex>
#include <thread>

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
            std::mutex _mutex;

            void createKitchen();
            void interpretMessage(std::string);
            void distributePizzas(Pizza, int &pizzanum);
            void messageInterpretorFunc();
    };
}

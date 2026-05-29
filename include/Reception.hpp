#pragma once

#include "Args.hpp"
#include "IPCM.hpp"
#include "PizzAbstract.hpp"
#include "SafeQueue.hpp"
#include <regex>

namespace plazza {
    class Reception {
        public:
            Reception() = delete;
            Reception(Args &args);
            ~Reception() = default;

            void run();

        private:
            struct KitchenContent {
                int pizzaAmount;
                pid_t pid;
            };

            double _multiplier;
            int _cooks;
            long long _restockDelay;
            bool _running;

            size_t _nextKitchenID;
            int _openedKitchen;
            std::regex _lineRegex;
            IPCM _ipc;
            plazza::SafeQueue<std::string> _messageQueue;
            plazza::SafeMap<size_t, KitchenContent> _kitchenMap;
            std::mutex _mutex;

            void createKitchen();
            void interpretMessage(std::string);
            void distributePizzas(std::shared_ptr<PizzInterface> pizza, int &pizzanum);
            void messageInterpretorFunc();
            void askStatus();
            void printStatus(std::vector<std::string>);
    };
}

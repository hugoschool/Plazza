#pragma once

#include "IPCM.hpp"
#include "PizzaParty.hpp"
#include "Stock.hpp"
#include <chrono>

namespace plazza {
    class Kitchen {
        public:
            Kitchen() = delete;
            Kitchen(double multiplier, int cooksAmount, long long restockDelay, size_t kitchenID, IPCM &);
            ~Kitchen() = default;

            void run();

        private:
            double _multiplier;
            int _cooksAmount;

            bool _running;
            size_t _kitchenID;
            IPCM &_ipc;

            std::chrono::steady_clock::time_point _creationTime;
            std::chrono::steady_clock::time_point _lastRestock;
            const std::chrono::seconds _expiry;
            const std::chrono::milliseconds _restockDelay;

            Stock _stock;
            PizzaParty _party;

            void createAndSendMessage(StatusCode code, std::optional<Pizza> pizza);
    };
}

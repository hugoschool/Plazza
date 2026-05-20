#pragma once

#include "IPCM.hpp"
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
            long long _restockDelay;
            size_t _kitchenID;
            IPCM &_ipc;

            std::chrono::steady_clock::time_point _lastBaked;
            const std::chrono::seconds _expiry;
    };
}

#pragma once

#include <chrono>

namespace plazza {
    class Kitchen {
        public:
            Kitchen() = delete;
            Kitchen(double multiplier, int cooksAmount, long long restockDelay);
            ~Kitchen() = default;

            void run();

        private:
            double _multiplier;
            int _cooksAmount;
            long long _restockDelay;

            std::chrono::steady_clock::time_point _lastBaked;
            const std::chrono::seconds _expiry;
    };
}

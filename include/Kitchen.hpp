#pragma once

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
    };
}

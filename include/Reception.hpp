#pragma once

#include "Args.hpp"
#include "Kitchen.hpp"
#include "IPCM.hpp"
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
            std::regex _lineRegex;
            IPCM _comeMeunier;

            std::vector<plazza::Kitchen> _kitchens;

            void createKitchen();
    };
}

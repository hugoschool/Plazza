#pragma once

#include "Pizza.hpp"

namespace plazza {
    class Cook {
        public:
            Cook() = default;
            ~Cook() = default;

            void execute(Pizza &, double);
    };
}

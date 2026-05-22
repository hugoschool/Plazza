#pragma once

#include "Pizza.hpp"
#include "Stock.hpp"

namespace plazza {
    class Cook {
        public:
            Cook(Stock &stock);
            ~Cook() = default;

            void execute(Pizza &, double);
        private:
            Stock &_stock;
    };
}

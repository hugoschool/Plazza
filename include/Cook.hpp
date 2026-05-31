#pragma once

#include "PizzAbstract.hpp"
#include "Stock.hpp"

namespace plazza {
    class Cook {
        public:
            Cook(Stock &stock);
            ~Cook() = default;

            void execute(PizzInterface &, double);
        private:
            Stock &_stock;
    };
}

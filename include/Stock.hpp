#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

namespace plazza {
    class Stock {
        public:
            static constexpr unsigned int MAX_UNIT_AMOUNT = 5;

            Stock();
            ~Stock() = default;

            void consume(std::string);
            void restock();

        private:
            std::unordered_map<std::string, unsigned int> _ingredients;
            std::mutex _mutex;

            void restockElement(unsigned int &);
    };
}

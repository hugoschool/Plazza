#pragma once

namespace plazza {
    class Stock {
        public:
            static constexpr unsigned int MAX_UNIT_AMOUNT = 5;

            Stock();
            ~Stock() = default;

            unsigned int dough;
            unsigned int tomato;
            unsigned int gruyere;
            unsigned int ham;
            unsigned int mushrooms;
            unsigned int steak;
            unsigned int eggplant;
            unsigned int goatCheese;
            unsigned int chiefLove;

            void restock();
            // TODO: definitely add a mutex here
            // TODO: maybe a CV too?

        private:
            void restockElement(unsigned int &);
    };
}

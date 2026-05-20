#pragma once

#include "Pizza.hpp"
#include <condition_variable>
#include <mutex>
#include <queue>
#include <thread>

namespace plazza {
    class PizzaParty {
        public:
            PizzaParty() = delete;
            PizzaParty(double multiplier, int cooksAmount);
            ~PizzaParty();

            void execute();
            void add(Pizza);

        private:
            const double _multiplier;
            const int _cooksAmount;

            bool _running;
            std::vector<std::thread> _threads;
            std::queue<Pizza> _pizzaQueue;
            std::condition_variable _cv;
            std::mutex _mutex;
    };
}

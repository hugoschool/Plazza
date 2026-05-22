#pragma once

#include "IPCM.hpp"
#include "Pizza.hpp"
#include <condition_variable>
#include <cstddef>
#include <mutex>
#include <queue>
#include <thread>

namespace plazza {
    class PizzaParty {
        public:
            PizzaParty() = delete;
            PizzaParty(double multiplier, int cooksAmount, Stock &stock, IPCM &ipc, size_t kitchenID);
            ~PizzaParty();

            void execute();
            void add(Pizza);

            std::chrono::steady_clock::time_point getLastBaked() const;

        private:
            const double _multiplier;
            const int _cooksAmount;
            Stock &_stock;
            IPCM &_ipc;
            size_t _kitchenID;

            bool _running;
            std::vector<std::thread> _threads;
            std::queue<Pizza> _pizzaQueue;
            std::condition_variable _cv;
            std::mutex _mutex;

            std::chrono::steady_clock::time_point _lastBaked;
    };
}

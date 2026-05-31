#pragma once

#include "Stock.hpp"

namespace plazza {
    class PizzInterface {
        public:
            virtual ~PizzInterface() = default;
            virtual bool getCooked() const = 0;
            virtual void setCooked(bool state) = 0;
            virtual double getBakingTime() const = 0;

            virtual std::string pack() const = 0;

            virtual void consume(Stock &) = 0;
            virtual std::string getTypeString(void) const = 0;
            virtual std::string getSizeString(void) const = 0;
    };
}

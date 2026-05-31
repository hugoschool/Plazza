#pragma once

#include "PizzaClass.hpp"

namespace plazza {
    class Builder {
    public:
        Builder() = default;
        ~Builder() = default;

        static std::shared_ptr<PizzInterface> buildPizza(pizza::Type type, pizza::Size size);
    };
}

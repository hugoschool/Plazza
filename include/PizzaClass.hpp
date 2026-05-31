#pragma once

#include "PizzAbstract.hpp"

namespace plazza {
    class Regina : public PizzAbstract {
    public:
        Regina() = delete;
        Regina(pizza::Size size);
        ~Regina() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
    };

    class Margarita : public PizzAbstract {
    public:
        Margarita() = delete;
        Margarita(pizza::Size size);
        ~Margarita() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
    };

    class Americana : public PizzAbstract {
    public:
        Americana() = delete;
        Americana(pizza::Size size);
        ~Americana() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
    };

    class Fantasia : public PizzAbstract {
    public:
        Fantasia() = delete;
        Fantasia(pizza::Size size);
        ~Fantasia() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
    };
}

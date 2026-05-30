#pragma once

#include "PizzAbstract.hpp"

namespace plazza {
    class Regina : public PizzAbstract {
    public:
        Regina() = delete;
        Regina(Size size);
        ~Regina() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
        std::string pack() const override;
    };

    class Margarita : public PizzAbstract {
    public:
        Margarita() = delete;
        Margarita(Size size);
        ~Margarita() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
        std::string pack() const override;
    };

    class Americana : public PizzAbstract {
    public:
        Americana() = delete;
        Americana(Size size);
        ~Americana() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
        std::string pack() const override;
    };

    class Fantasia : public PizzAbstract {
    public:
        Fantasia() = delete;
        Fantasia(Size size);
        ~Fantasia() = default;

        std::string getTypeString(void) const override;
        double getBakingTime() const override;
        void consume(Stock &stock) override;
        std::string pack() const override;
    };
}

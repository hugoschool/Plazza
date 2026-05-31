#pragma once

#include "PizzaEnums.hpp"
#include "Stock.hpp"
#include "PizzInterface.hpp"
#include <optional>
#include <ostream>
#include <regex>
#include <string>
#include <sstream>

namespace plazza {
    class PizzAbstract : public PizzInterface {
        public:
            PizzAbstract() = delete;
            PizzAbstract(pizza::Size size, pizza::Type type);
            ~PizzAbstract() = default;

            bool getCooked() const override;
            void setCooked(bool state) override;

            // Time is given in milliseconds. The multiplier must be added on top.
            virtual double getBakingTime() const override = 0;
            virtual std::string getTypeString(void) const override = 0;
            virtual void consume(Stock &) override = 0;

            static std::optional<pizza::Type> getType(std::string);
            static std::string getType(pizza::Type);

            static std::optional<pizza::Size> getSize(std::string);
            static std::string getSize(pizza::Size);

            std::string pack() const override;
            static std::shared_ptr<PizzInterface> unpack(std::string);
            static std::shared_ptr<PizzInterface> unpack(std::smatch);

            std::string getSizeString(void) const override;

        private:
            bool _cooked;
            pizza::Type _type;

            template<typename T>
            static std::optional<T> retrieveNb(std::string str)
            {
                int nb = 0;

                std::stringstream ss(str);
                ss >> nb;

                if (ss.fail() || nb <= 0)
                    return std::nullopt;

                return static_cast<T>(nb);
            }

        protected:
            pizza::Size _size;
    };
}

std::ostream &operator<<(std::ostream &s, const std::shared_ptr<plazza::PizzInterface> pizza);

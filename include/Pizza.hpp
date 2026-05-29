#pragma once

#include "Stock.hpp"
#include <optional>
#include <ostream>
#include <regex>
#include <string>
#include <sstream>

namespace plazza {
    class Pizza {
        public:
            enum class Type
            {
                Regina = 1,
                Margarita = 2,
                Americana = 4,
                Fantasia = 8
            };

            enum class Size
            {
                S = 1,
                M = 2,
                L = 4,
                XL = 8,
                XXL = 16
            };

            Pizza() = delete;
            Pizza(Type type, Size size);
            ~Pizza() = default;

            bool getCooked() const;
            void setCooked(bool state);

            // Time is given in milliseconds. The multiplier must be added on top.
            double getBakingTime() const;

            static std::optional<Type> getType(std::string);
            static std::string getType(Type);

            static std::optional<Size> getSize(std::string);
            static std::string getSize(Size);

            std::string pack() const;
            static std::optional<Pizza> unpack(std::string);
            static std::optional<Pizza> unpack(std::smatch);

            void consume(Stock &);
            std::string getTypeString(void) const;
            std::string getSizeString(void) const;

        private:
            Type _type;
            Size _size;

            bool _cooked;

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
    };
}

std::ostream &operator<<(std::ostream &s, const plazza::Pizza&);
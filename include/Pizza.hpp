#pragma once

#include <optional>
#include <string>

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
        private:
            Type _type;
            Size _size;

            bool _cooked;
    };
}

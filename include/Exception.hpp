#pragma once

#include <exception>
#include <string>

namespace plazza {
    class Exception : public std::exception {
        public:
            Exception(const std::string str) : _str(str)
            {
            }

            ~Exception() = default;

            const char *what() const noexcept override
            {
                return _str.c_str();
            }

        private:
            const std::string _str;
    };
}

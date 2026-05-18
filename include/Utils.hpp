#pragma once

#include <algorithm>
#include <string>
#include <vector>

namespace plazza::Utils {
    class String {
        public:
            static std::vector<std::string> split(std::string s, const std::string &delimiter) {
                std::vector<std::string> tokens;
                std::size_t pos = 0;
                std::string token;

                while ((pos = s.find(delimiter)) != std::string::npos) {
                    token = s.substr(0, pos);
                    tokens.push_back(token);
                    s.erase(0, pos + delimiter.length());
                }
                tokens.push_back(s);
                return tokens;
            }

            static std::string tolower(std::string str) {
                std::string newString(str);

                std::transform(newString.begin(), newString.end(), newString.begin(), [](unsigned char c) {
                    return std::tolower(c);
                });
                return newString;
            }
    };
}

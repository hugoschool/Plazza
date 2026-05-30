#pragma once

#include <string>
#include <vector>

namespace plazza {
    class Args {
        public:
            Args() = delete;
            Args(int argc, char **argv);
            ~Args() = default;

            bool isHelp() const;
            double getMultiplier() const;
            int getCooks() const;
            long long getRestockDelay() const;
        private:
            const int _argc;
            std::vector<std::string> _argv;

            bool _help;
    };
}

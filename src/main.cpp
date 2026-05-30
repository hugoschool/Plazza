#include "Args.hpp"
#include "Reception.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    try {
        plazza::Args args(argc, argv);
        if (args.isHelp())
            return 0;

        plazza::Reception reception(args);

        reception.run();
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}

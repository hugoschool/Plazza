#include "Args.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    try {
        plazza::Args args(argc, argv);
    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
}

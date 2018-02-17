//
// Created by Kévin POLOSSAT on 12/02/2018.
//

#include <iostream>
#include "Server.h"

using namespace lw_network;
int main(int ac, char **av) {
    try {
        Server s;
        s.run();
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

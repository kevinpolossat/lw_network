//
// Created by Kévin POLOSSAT on 15/01/2018.
//

#include <Resolver.h>
#include <array>
#include <iostream>
#include "Socket.h"

int main() {
    lw_network::Socket socket;
    lw_network::Resolver re;
    re
            .SetService("4242")
            .SetFamily(AF_UNSPEC)
            .SetSockType(SOCK_DGRAM)
            .SetFlags(AI_PASSIVE);

    for (auto & endPoint: re.Resolve()) {
        auto e = lw_network::no_error;
        socket.open(endPoint.protocol(), e);
        if (e) {
            continue;
        }
        socket.bind(endPoint, e);
        if (e) {
            socket.close(e);
            continue;
        }
        break;
    }
    if (!socket.isOpen()) {
        perror("::");
        return 0;
    }
    std::array<char, 256> b;
    lw_network::EndPoint ep;
    lw_network::Buffer buffer(b.data(), b.size());
    auto ec = lw_network::no_error;
    auto n = socket.recvfrom(ep, buffer, 0, ec);
    if (ec) {
        perror("sendto::");
    }
    std::string s(b.data(), n);
    std::cout << "[" << s << "]" << std::endl;
}
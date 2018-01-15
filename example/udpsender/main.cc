#include <Resolver.h>
#include "Socket.h"

int main() {
    lw_network::Socket socket;
    lw_network::Resolver re;
    re
            .SetNode("localhost")
            .SetService("4242")
            .SetFamily(AF_UNSPEC)
            .SetSockType(SOCK_DGRAM)
            .SetFlags(AI_PASSIVE);
    lw_network::EndPoint ep;
    for (auto & endPoint: re.Resolve()) {
        auto e = lw_network::no_error;
        socket.open(endPoint.protocol(), e);
        if (e) {
            continue;
        }
        ep = endPoint;
        break;
    }
    if (!socket.isOpen()) {
        perror("open::");
        return 0;
    }
    std::string s = "Hello world !";
    lw_network::Buffer buffer(const_cast<char *>(s.data()), s.size());
    auto ec = lw_network::no_error;
    socket.sendto(ep, buffer, 0, ec);
    if (ec) {
        perror("sendto::");
    }
}
//
// Created by Kévin POLOSSAT on 08/01/2018.
//

#include <iostream>
#include <arpa/inet.h>
#include "resolver.h"
#include "socket_operations.h"
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main() {
    lw_network::Resolver re;
    re
            .SetService("4242")
            .SetFamily(AF_UNSPEC)
            .SetSockType(SOCK_STREAM)
            .SetFlags(AI_PASSIVE);
    socket_type listener;
    int yes = 1;
    lw_network::error_code e = lw_network::no_error;
    auto p = re.Resolve();
    bool f = false;
    for (auto const & endPoint : p) {
        e = lw_network::no_error;
        listener = lw_network::socket_operations::socket(endPoint.protocol().family(), endPoint.protocol().type(), endPoint.protocol().protocol(), e);
        if (e) {
            continue;
        }
        lw_network::socket_operations::setsockopt<socklen_t>(listener, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int), e);
        lw_network::socket_operations::bind<socklen_t>(listener, endPoint.Data(), endPoint.Size(), e);
        if (e) {
            lw_network::socket_operations::close(listener, e);
            continue;
        }
        f = true;
        break;
    }
    if (!f) {
        std::cout << "FAIL" << std::endl;
        return 0;
    }
    lw_network::socket_operations::listen(listener, 10, e);
    if (e) {
        std::cout << "FAIL listen" << std::endl;
        return 0;
    }
    struct sockaddr_storage remoteaddr;
    std::size_t addrlen = sizeof remoteaddr;
    char remoteIP[INET6_ADDRSTRLEN];
    for (;;) {
        socket_type peer = lw_network::socket_operations::accept<socklen_t>(listener, reinterpret_cast<struct sockaddr *>(&remoteaddr), &addrlen, e);
        printf("selectserver: new connection from %s on "
                       "socket %d\n",
               inet_ntop(remoteaddr.ss_family,
                         get_in_addr((struct sockaddr*)&remoteaddr),
                         remoteIP, INET6_ADDRSTRLEN),
               peer);
        lw_network::socket_operations::close(peer, e);
    }
    return 0;
}
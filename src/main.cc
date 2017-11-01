//
// Created by Kévin POLOSSAT on 01/11/2017.
//

#include <arpa/inet.h>
#include <iostream>

#include "socket.h"
#include "resolver.h"

int main(int ac, char *av[]) {
    lw_tcp_server::Socket s;
    lw_tcp_server::Socket peer;
    lw_tcp_server::Resolver r;

    char ipstr[INET6_ADDRSTRLEN];
    auto end_points = r.Resolve("www.example.net", "", AF_UNSPEC, SOCK_STREAM, 0, AI_PASSIVE);
    for (auto i = end_points; i; ++i) {
        void *addr;
        std::string ipver;

        // get the pointer to the address itself,
        // different fields in IPv4 and IPv6:
        if (i->ai_family == AF_INET) { // IPv4
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)i->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else { // IPv6
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)i->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }

        // convert the IP to a string and print it:
        inet_ntop(i->ai_family, addr, ipstr, sizeof ipstr);
        std::cout << ipver << ": " << ipstr << std::endl;
    }
    return 0;
}
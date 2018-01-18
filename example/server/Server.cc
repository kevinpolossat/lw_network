//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#if defined (__linux__) || defined (__APPLE__)
#include <arpa/inet.h>
#elif defined (_WIN32) || defined (_WIN64)
#include <WS2tcpip.h>
#else
#error "unknown platform"
#endif

#include <memory>
#include <iostream>
#include "Server.h"
#include "Resolver.h"

Server::Server(): reactor_(), acceptor_(reactor_) {
    lw_network::Reactor reactor;
    lw_network::Resolver re;
    re
            .SetService("4242")
            .SetFamily(AF_UNSPEC)
            .SetSockType(SOCK_STREAM)
            .SetFlags(AI_PASSIVE);
    int yes = 1;
    lw_network::error_code e = lw_network::no_error;
    auto p = re.Resolve();
    for (auto const & endPoint : p) {
        e = lw_network::no_error;
        acceptor_.open(endPoint.protocol(), e);
        if (e) {
            continue;
        }
        acceptor_.setOption(SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int), e);
        acceptor_.bind(endPoint, e);
        if (e) {
            acceptor_.close(e);
            continue;
        }
        break;
    }
    if (!acceptor_.isOpen()) {
        std::cout << "FAIL" << std::endl;
        return ;
    }
    acceptor_.listen(10, e);
    if (e) {
        std::cout << "FAIL listen" << std::endl;
        return ;
    }
    doAccept();
}

void *Server::get_in_addr_(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void Server::run() {
    reactor_.handleEvents();
}

void Server::doAccept() {
    acceptor_.asyncAccept(
            [this](lw_network::ReactiveSocket peer, lw_network::error_code ec) {
                char remoteIP[INET6_ADDRSTRLEN];
                lw_network::error_code e = lw_network::no_error;
                auto peerPoint = peer.remoteEndPoint(e);

#if defined (__linux__) || defined (__APPLE__)
				auto *out = inet_ntop(
					peerPoint.Data()->sa_family,
					this->get_in_addr_(peerPoint.Data()),
					remoteIP,
					INET6_ADDRSTRLEN);
#elif defined (_WIN32) || defined (_WIN64)
				auto *out = InetNtop(
					peerPoint.Data()->sa_family,
					this->get_in_addr_(peerPoint.Data()),
					remoteIP,
					INET6_ADDRSTRLEN);
#else
#error "unknown platform"
#endif


				std::cout << "new connection from "
                          << out 
                          << std::endl;
                manager_.start(std::make_shared<Connection>(std::move(peer), manager_));
                this->doAccept();
            }
    );

}

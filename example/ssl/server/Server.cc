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
#include <SSLSocket.h>
#include <openssl/err.h>
#include "Server.h"
#include "Resolver.h"

Server::Server() : reactor_(), acceptor_(reactor_), ctx_(lw_network::SSLContext::Method::SSLv23_server) {
    lw_network::error_code e = lw_network::no_error;
    ctx_.useCertificateFile("mycert.pem", lw_network::SSLContext::FileFormat::PEM);
    ctx_.usePrivateKeyFile("mycert.pem", lw_network::SSLContext::FileFormat::PEM);

    lw_network::Reactor reactor;
    lw_network::Resolver re;
    re
            .SetService("4242")
            .SetFamily(AF_UNSPEC)
            .SetSockType(SOCK_STREAM)
            .SetFlags(AI_PASSIVE);
    int yes = 1;
    auto p = re.Resolve();
    for (auto const &endPoint : p) {
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
        return;
    }
    acceptor_.listen(SOMAXCONN, e);
    if (e) {
        std::cout << "FAIL listen" << std::endl;
        return;
    }
    doAccept();
}

void *Server::get_in_addr_(struct sockaddr *sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in *) sa)->sin_addr);
    }
    return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}

void Server::run() {
    reactor_.handleEvents();
}

void Server::doAccept() {
    acceptor_.asyncAccept(
            [this](lw_network::ReactiveSocketBase<lw_network::SSLSocket> peer, lw_network::error_code ec) { // CHECK ERROR !!
                lw_network::error_code e = lw_network::no_error;
                peer.setContext(ctx_);
                peer.initSession(e);
                if (e != lw_network::no_error) {
                    ERR_print_errors_fp(stderr);
                    peer.close();
                    this->doAccept();
                    return ;
                }
                peer.serverHandShake(e);
                if (e == -1) {
                    ERR_print_errors_fp(stderr);
                    peer.close();
                    this->doAccept();
                    return ;
                }
                std::array<char, 1025> buf;
                lw_network::Buffer b(buf.data(), sizeof(buf));
                auto nrbyte = peer.read(b, e);
                if (nrbyte > 0) {
                    std::string s;
                    std::copy(buf.begin(), buf.begin() + nrbyte, std::back_inserter(s));
                    std::cout << "request:\"" << s << "\"" << std::endl;
                }
                    peer.close();
                //manager_.start(std::make_shared<Connection>(std::move(peer), manager_));
                this->doAccept();
            }
    );
}

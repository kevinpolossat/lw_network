//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#ifndef LW_TCP_SERVER_SERVER_H
#define LW_TCP_SERVER_SERVER_H


#include <Reactor.h>
#include <AcceptorBase.h>
#include "ConnectionManager.h"
#include "SSLContext.h"

class Server {
public:
    Server();
    void run();
private:
    void doAccept();
    void *get_in_addr_(struct sockaddr *sa);

private:
    lw_network::Reactor reactor_;
    lw_network::SSLAcceptor acceptor_;
    lw_network::SSLContext ctx_;
    ConnectionManager manager_;

};


#endif //LW_TCP_SERVER_SERVER_H

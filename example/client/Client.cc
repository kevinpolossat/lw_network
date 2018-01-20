//
// Created by Kévin POLOSSAT on 15/01/2018.
//

#include <iostream>
#include <Resolver.h>
#include "Client.h"

void Client::start(std::string const & host, std::string const & port) {
	auto c = connect_(host, port);
    if (!c) {
        std::cerr << "can't connect to remote host " << host << std::endl;
    }
    else {
        std::cout << "client connected" << std::endl;
        std::string input;
        lw_network::error_code e = lw_network::no_error;
        s_.nonBlocking(true, e);
        if (e) {
            perror("nonblocking:");
            return;
        }
        for (;;) {
            e = lw_network::no_error;
            std::getline(std::cin, input);
            std::cout << "===>" << input << std::endl;
            lw_network::Buffer b(static_cast<void *>(const_cast<char *>(input.data())), input.size());
			std::cout << "ici" << std::endl;
			s_.send(b, 0, e);
			std::cout << "out ==> " << e << std::endl;
			if (e) {
                perror("send:");
            }
        }
    }
}

bool Client::connect_(std::string const & host, std::string const & port) {
    lw_network::Resolver re;
	re
		.SetNode(host)
		.SetService(port)
		.SetFamily(AF_UNSPEC)
		.SetSockType(SOCK_STREAM);
    int yes = 1;
    for (auto & endPoint: re.Resolve()) {
        auto e = lw_network::no_error;
        s_.open(endPoint.protocol(), e);
        if (e) {
            continue;
        }
        s_.connect(endPoint, e);
        if (e) {
			s_.close(e);
            continue;
        }
        break;
    }
    return s_.isOpen();
}

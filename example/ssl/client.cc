//
// Created by Kévin POLOSSAT on 12/02/2018.
//

#include <Resolver.h>
#include <iostream>
#include <SSLSocket.h>
#include <openssl/err.h>

using namespace lw_network;
int main(int ac, char **av) {
    if (ac != 5) {
        std::cout << "usage: " << *av << " server_domain server_port certificate_path private_key_path" << std::endl;
        return 1;
    }
    SSLContext ctx(SSLContext::Method::SSLv3_client);
    ctx.useCertificateFile(av[3], SSLContext::FileFormat::PEM);
    ctx.usePrivateKeyFile(av[4], SSLContext::FileFormat::PEM);
    SSLSocket socket(ctx);
    lw_network::Resolver re;
    re
            .SetNode(av[1])
            .SetService(av[2])
            .SetFamily(AF_UNSPEC)
            .SetSockType(SOCK_STREAM);
    int yes = 1;
    for (auto & endPoint: re.Resolve()) {
        auto e = lw_network::no_error;
        socket.open(endPoint.protocol(), e);
        if (e) {
            continue;
        }
        socket.connect(endPoint, e);
        if (e) {
            socket.close(e);
            continue;
        }
        break;
    }
    if (!socket.isOpen()) {
        perror("open");
        return 1;
    }
    error_code e = no_error;
    socket.initSession(e);
    if (e != -1) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    socket.clientHandShake(e);
    if (e != -1) {
        ERR_print_errors_fp(stderr);
        return 1;
    }
    return 0;
}
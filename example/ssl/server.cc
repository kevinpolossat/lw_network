//
// Created by Kévin POLOSSAT on 12/02/2018.
//

#include <SSLSocket.h>
#include <AcceptorBase.h>
#include <Resolver.h>
#include <iostream>
#include <openssl/err.h>
#include <Buffer.h>

using namespace lw_network;
int main(int ac, char **av) {
    if (ac != 3) {
        std::cerr << "usage: " << *av << " certificate_path private_key_path" << std::endl;
        return 1;
    }
    try {
        error_code e = no_error;
        SSLContext ctx(SSLContext::Method::SSLv3_server);
        ctx.useCertificateFile(av[1], SSLContext::FileFormat::PEM);
        ctx.usePrivateKeyFile(av[2], SSLContext::FileFormat::PEM);
        Socket listener;
        Resolver re;
        re
                .SetService("4242")
                .SetFamily(AF_UNSPEC)
                .SetSockType(SOCK_STREAM)
                .SetFlags(AI_PASSIVE);
        int yes = 1;
        auto p = re.Resolve();
        for (auto const & endPoint : p) {
            e = lw_network::no_error;
            listener.open(endPoint.protocol(), e);
            if (e) {
                continue;
            }
            listener.setOption(SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int), e);
            listener.bind(endPoint, e);
            if (e) {
                listener.close(e);
                continue;
            }
            break;
        }
        if (!listener.isOpen()) {
            std::cout << "FAIL" << std::endl;
            return 1;
        }
        listener.listen(SOMAXCONN, e);
        if (e) {
            std::cout << "FAIL listen" << std::endl;
            return 1;
        }
        std::cout << "server on port 4242" << std::endl;
        std::string s;
        s.reserve(1024);
        for (;;) {
            SSLSocket sslSocket(ctx);
            listener.accept(sslSocket, e);
            std::cout << "::ici::" << std::endl;
            if (e != no_error) {
                perror("accept");
                continue ;
            }
            sslSocket.initSession(e);
            std::cout << "::ici::" << std::endl;
            if (e != -1) {
                ERR_print_errors_fp(stderr);
                continue ;
            }
            sslSocket.serverHandShake(e);
            if (e != -1) {
                ERR_print_errors_fp(stderr);
                continue ;
            }
            Buffer b(const_cast<char *>(s.data()), s.capacity());
            auto nrbyte = sslSocket.read(b, e);
            std::cout << "[" << s << "]" << std::endl;
            auto nwbyte = sslSocket.write(b, e);
        }
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

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
        SSLContext ctx(SSLContext::Method::SSLv23_server);
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
        for (;;) {
            e = lw_network::no_error;
            SSLSocket sslSocket(ctx);
            listener.accept(sslSocket, e);

            if (e != no_error) {
                perror("accept");
                continue ;
            }
            sslSocket.initSession(e);
            if (e != lw_network::no_error) {
                ERR_print_errors_fp(stderr);
                continue ;
            }
            sslSocket.serverHandShake(e);
            if (e == -1) {
                ERR_print_errors_fp(stderr);
                continue ;
            }
            else {
                std::array<char, 1025> buf;
                Buffer b(buf.data(), sizeof(buf));
                auto nrbyte = sslSocket.read(b, e);
                if (nrbyte > 0) {
                    std::string s;
                    std::copy(buf.begin(), buf.begin() + nrbyte, std::back_inserter(s));
                    std::cout << "request:\"" << s << "\"" << std::endl;
                    //auto nwbyte = sslSocket.write(b, e);
                }
            }
            sslSocket.close(e);
        }
    }
    catch (std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}

//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#ifndef LW_TCP_SERVER_SSLINIT_H
#define LW_TCP_SERVER_SSLINIT_H

namespace lw_network {
class SSLInit {
public:
    ~SSLInit() = default;

    static SSLInit &instance();

private:
    SSLInit();

    SSLInit(SSLInit const &other) = delete;

    SSLInit(SSLInit &&other) = delete;

    SSLInit &operator=(SSLInit const &other) = delete;

    SSLInit &operator=(SSLInit &&other) = delete;
};
}

#endif //LW_TCP_SERVER_SSLINIT_H

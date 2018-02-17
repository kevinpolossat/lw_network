//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#include "SSLHeader.h"
#include "SSLInit.h"

lw_network::SSLInit &lw_network::SSLInit::instance() {
    static SSLInit instance;
    return instance;
}

lw_network::SSLInit::SSLInit() {
    ::SSL_load_error_strings();
    ::OpenSSL_add_ssl_algorithms();
}

//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#include <openssl/ssl.h>
#include <openssl/err.h>
#include "SSLInit.h"

SSLInit &SSLInit::instance() {
    static SSLInit instance;
    return instance;
}

SSLInit::SSLInit() {
    ::SSL_load_error_strings();
    ::OpenSSL_add_ssl_algorithms();
}

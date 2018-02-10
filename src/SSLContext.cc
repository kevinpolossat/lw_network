//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <exception>
#include "SSLContext.h"

std::array<lw_network::SSLContext::SSLMethodBuilder, 12> const lw_network::SSLContext::methodBuilder_ = {
        SSLv2_method,
        SSLv2_server_method,
        SSLv2_client_method,
        SSLv3_method,
        SSLv3_server_method,
        SSLv3_client_method,
        TLSv1_method,
        TLSv1_server_method,
        TLSv1_client_method,
        SSLv23_method,
        SSLv23_server_method,
        SSLv23_client_method
};

lw_network::SSLContext::SSLContext(lw_network::SSLContext::Method m) throw(std::runtime_error) {
    ctx_ = std::shared_ptr<SSL_CTX>(
            SSL_CTX_new(methodBuilder_[static_cast<int>(m)]()),
            [](auto ctxPtr) {
                SSL_CTX_free(ctxPtr);
            });
    if (!ctx_) {
        ERR_print_errors_fp(stderr);
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void lw_network::SSLContext::useCertificateFile(std::string_view file, lw_network::SSLContext::FileFormat pem) throw(std::runtime_error) {
    if (::SSL_CTX_use_certificate_file(
            ctx_.get(),
            file.data(),
            pem == lw_network::SSLContext::FileFormat::PEM ? SSL_FILETYPE_PEM : SSL_FILETYPE_ASN1) <= 0) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void lw_network::SSLContext::useCertificateChainFile(std::string_view file) throw(std::runtime_error) {
    if (::SSL_CTX_use_certificate_chain_file(ctx_.get(), file.data()) <= 0) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void lw_network::SSLContext::usePrivateKeyFile(std::string_view file, lw_network::SSLContext::FileFormat pem) throw(std::runtime_error) {
    if (SSL_CTX_use_PrivateKey_file(
            ctx_.get(),
            file.data(),
            pem == lw_network::SSLContext::FileFormat::PEM ? SSL_FILETYPE_PEM : SSL_FILETYPE_ASN1) <= 0
        || !checkPrivateKey_()) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

bool lw_network::SSLContext::checkPrivateKey_() {
    return SSL_CTX_check_private_key(ctx_.get()) != 0;
}

//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#include <openssl/ssl.h>
#include <openssl/err.h>
#include <exception>
#include "SSLContext.h"

std::array<SSLContext::SSLMethodBuilder, 12> const SSLContext::methodBuilder_ = {
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

SSLContext::SSLContext(SSLContext::Method m) throw(std::runtime_error) {
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

void SSLContext::useCertificateFile(std::string_view file, SSLContext::FileFormat pem) throw(std::runtime_error) {
    if (::SSL_CTX_use_certificate_file(
            ctx_.get(),
            file.data(),
            pem == SSLContext::FileFormat::PEM ? SSL_FILETYPE_PEM : SSL_FILETYPE_ASN1) <= 0) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void SSLContext::useCertificateChainFile(std::string_view file) throw(std::runtime_error) {
    if (::SSL_CTX_use_certificate_chain_file(ctx_.get(), file.data()) <= 0) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void SSLContext::usePrivateKeyFile(std::string_view file, SSLContext::FileFormat pem) throw(std::runtime_error) {
    if (SSL_CTX_use_PrivateKey_file(
            ctx_.get(),
            file.data(),
            pem == SSLContext::FileFormat::PEM ? SSL_FILETYPE_PEM : SSL_FILETYPE_ASN1) <= 0
        || !checkPrivateKey_()) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

bool SSLContext::checkPrivateKey_() {
    return SSL_CTX_check_private_key(ctx_.get()) != 0;
}

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

lw_network::SSLContext::SSLContext(): sslInit_(lw_network::SSLInit::instance()) {}

lw_network::SSLContext::SSLContext(lw_network::SSLContext::Method m): sslInit_(lw_network::SSLInit::instance()) {
    this->fromMethod(m);
}

void lw_network::SSLContext::useCertificateFile(std::string const & file, lw_network::SSLContext::FileFormat pem) {
    if (::SSL_CTX_use_certificate_file(
            ctx_.get(),
            file.data(),
            pem == lw_network::SSLContext::FileFormat::PEM ? SSL_FILETYPE_PEM : SSL_FILETYPE_ASN1) <= 0) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void lw_network::SSLContext::useCertificateChainFile(std::string const & file) {
    if (::SSL_CTX_use_certificate_chain_file(ctx_.get(), file.data()) <= 0) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void lw_network::SSLContext::usePrivateKeyFile(std::string const & file, lw_network::SSLContext::FileFormat pem) {
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

lw_network::SSLContext::SSLContext(
        const lw_network::SSLContext &other): ctx_(other.ctx_), sslInit_(SSLInit::instance()) {}

lw_network::SSLContext::SSLContext(
        lw_network::SSLContext &&other): ctx_(std::move(other.ctx_)), sslInit_(SSLInit::instance()) {}

lw_network::SSLContext &lw_network::SSLContext::operator=(const lw_network::SSLContext &other) {
    ctx_ = other.ctx_;
    return *this;
}

lw_network::SSLContext &lw_network::SSLContext::operator=(lw_network::SSLContext &&other) {
    ctx_ = std::move(other.ctx_);
    return *this;
}

void lw_network::SSLContext::loadVerifyFile(std::string const & file, std::string const & path) {
    if (::SSL_CTX_load_verify_locations(ctx_.get(), file.data(), path.empty() ? nullptr : path.data()) != 0) {
        throw std::runtime_error(ERR_error_string(ERR_get_error(), nullptr));
    }
}

void lw_network::SSLContext::setVerifyMode(lw_network::SSLContext::Mode mode) {
    SSL_CTX_set_verify(ctx_.get(), static_cast<int>(mode), nullptr/*TODO SET*/);
}

void lw_network::SSLContext::setVerifyDepth(std::uint32_t depth) {
    SSL_CTX_set_verify_depth(ctx_.get(), depth);
}

SSL_CTX *lw_network::SSLContext::getLowLevelContext() {
    return ctx_.get();
}

void lw_network::SSLContext::fromMethod(lw_network::SSLContext::Method m) {
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

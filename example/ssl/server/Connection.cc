//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "ConnectionManager.h"

Connection::Connection(lw_network::ReactiveSocket s, ConnectionManager &cm): s_(std::move(s)), cm_(cm) {}

void Connection::start() {
    doRead_();
}

void Connection::stop() {
    s_.close();
}

void Connection::doWrite_() {
    s_.async_write(
            lw_network::Buffer(const_cast<void *>(reinterpret_cast<void const *>(bufferWrite_.data())), bufferWrite_.size()),
            [this](std::size_t, lw_network::error_code) {
                doRead_();
            }
    );
}

void Connection::doRead_() {
    s_.async_read_some(
            lw_network::Buffer(bufferRead_.data(), bufferRead_.size()),
            [this](std::size_t nbyte, lw_network::error_code ec) {
                this->bufferWrite_.assign(this->bufferRead_.data(), nbyte);
                std::cout << "[" << this->bufferWrite_ << "]" << std::endl;
                doRead_();
//                this->doWrite_();
            }
    );
}

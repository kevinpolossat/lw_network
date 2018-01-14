//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#include <iostream>
#include "Buffer.h"
#include "Connection.h"
#include "ConnectionManager.h"

Connection::Connection(lw_network::Socket s, ConnectionManager &cm): s_(std::move(s)), cm_(cm), greeting_("Hello world !\r\n") {}

void Connection::start() {
    doWrite();
}

void Connection::stop() {
    s_.close();
}

void Connection::doWrite() {
    s_.async_write(
            lw_network::Buffer(const_cast<void *>(reinterpret_cast<void const *>(greeting_.data())), greeting_.size()),
            [this](std::size_t, lw_network::error_code) {
                this->cm_.stop(shared_from_this());
            }
    );
}

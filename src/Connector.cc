//
// Created by Kévin POLOSSAT on 14/01/2018.
//

#include "Connector.h"

lw_network::Connector::Connector(lw_network::Reactor &re) : Socket(re) {}

void lw_network::Connector::connect(const lw_network::Resolver &resolver) {
    for (auto & ep : resolver.Resolve()) {
        error_code ec;
        Socket::connect(ep, ec);
        if (ec != no_error) {
            continue;
        }
        break;
    }
}
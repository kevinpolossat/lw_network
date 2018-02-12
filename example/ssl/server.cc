//
// Created by Kévin POLOSSAT on 12/02/2018.
//

#include <SSLSocket.h>

int main() {
    lw_network::SSLContext ctx(lw_network::SSLContext::Method::SSLv3_server);
    lw_network::SSLSocket s(ctx);
    return 0;
}

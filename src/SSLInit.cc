//
// Created by Kévin POLOSSAT on 10/02/2018.
//

#include "SSLInit.h"

SSLInit::~SSLInit() {

}

SSLInit &SSLInit::instance() {
    static SSLInit instance;
    return instance;
}

SSLInit::SSLInit() {

}

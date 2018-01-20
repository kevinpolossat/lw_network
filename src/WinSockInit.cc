//
// Created by Kévin POLOSSAT on 20/01/2018.
//

#include "WinSockInit.h"

#if defined (_WIN32) || defined (_WIN64)
const lw_network::WinSockInit wsaInit;
#endif //defined (_WIN32) || defined (_WIN64)



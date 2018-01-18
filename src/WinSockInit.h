#ifndef WINSOCK_INIT_H_
#define WINSOCK_INIT_H_

#if defined (_WIN32) || defined (_WIN64)

#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "wsock32.lib")
#pragma comment(lib, "ws2_32.lib")

class WinSockInit {
public:
	WinSockInit() {
		WSAData wsadata;
		::WSAStartup(MAKEWORD(2, 2), &wsadata);
	}
	~WinSockInit() {
		WSACleanup();
	}
};

static const WinSockInit wsaInit;

#endif //defined (_WIN32) || defined (_WIN64)

#endif //WINSOCK_INIT_H_
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")
#include <iostream>
#include "winsock2.h"

#include "Constants.h"
#include "Actions.h"

class Connection {
	SOCKET socket;
	sockaddr_in addr;

public:
	const uint16_t id;
	Connection();
	Connection(SOCKET socket, sockaddr_in addr);

	void send_(Action, uint8_t*, size_t);
	Action recv_(uint8_t*, size_t);

};


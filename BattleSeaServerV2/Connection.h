#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")
#include <iostream>
#include "winsock2.h"

#include "Constants.h"
#include "Actions.h"
#include "Errors.h"

using namespace std;

class Connection {
	SOCKET socket_;
	sockaddr_in addr_;
	bool connected;

public:
	const uint16_t id;
	Connection();
	Connection(SOCKET socket, sockaddr_in addr);

	bool isConnected() const;

	void send_(Action, uint8_t* = 0, size_t = 0);
	Action recv_(uint8_t* = 0, size_t = 0);

	void close();

};


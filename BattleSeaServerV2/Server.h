#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment (lib, "ws2_32.lib")
#include <iostream>
#include <thread>
#include "winsock2.h"
#include <windows.h>
#include <vector>
//#include <chrono>
//#include <cmath>
#include <time.h>

#include "Connection.h"

using namespace std;
using namespace std::chrono;

class Server {
	SOCKET thisSocket;
	WSAData thisWsaData;
	sockaddr_in localAddr;
	uint32_t port;
	vector<Connection*> connections;
public:
	Server(uint32_t);

	void start();
	void close();

	static void connectionHandler(Connection);
	static void pingHandler(Connection);
};


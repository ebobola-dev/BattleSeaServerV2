#include "Server.h"

Server::Server(uint32_t port) {
	this->port = port;
	if (WSAStartup(MAKEWORD(2, 2), &thisWsaData)) close();
	if ((thisSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) close();
	localAddr.sin_family = AF_INET;
	localAddr.sin_port = htons(port);
	localAddr.sin_addr.s_addr = 0;
	if (bind(thisSocket, (sockaddr*)&localAddr, sizeof(localAddr))) close();
	if (listen(thisSocket, 0x100)) close();
}

void Server::start() {
	printf("Start listening on port: %u\n", port);
	SOCKET client_socket;
	sockaddr_in client_addr;
	int client_addr_size = sizeof(client_addr);
	while ((client_socket = accept(thisSocket, (sockaddr*)&client_addr, &client_addr_size))) {
		Connection newConnection(client_socket, client_addr);
		printf("New connection! - [%u]\n", newConnection.id);
		connections.push_back(&newConnection);
		thread newConnectionThread(connectionHandler, newConnection);
		newConnectionThread.detach();
	}
}

void Server::close() {
	closesocket(thisSocket);
	WSACleanup();
}

void Server::connectionHandler(Connection connection) {
	thread newConnectionThread(pingHandler, connection);
	newConnectionThread.join();
}

void Server::pingHandler(Connection connection) {
	while (connection.isConnected()) {
		connection.send_(Action::ping);
		clock_t pingTime = clock();
		cout << "sent" << endl;
		while (1) {
			Action act = connection.recv_();
			cout << "recv: " << (int)act << endl;
			if (act == Action::pong) break;
		}
		double pongTime = (double)(clock() - pingTime);
		cout << "pongTime: " << pongTime << endl;
		/*uint8_t pingArr[8];
		Action act = connection.recv_(pingArr, 3);
		if (act != Action::ping) continue;
		milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
		uint64_t ms_ = ms.count();
		uint64_t connectionTime = 0;
		for (size_t i = 0; i < 7; i++) {
			connectionTime += pingArr[i + 1] * (uint64_t)pow(100, 6 - i);
		}
		uint64_t delta = ms_ - connectionTime;
		cout << "ms_: " << ms_ << ", connectionTime: " << connectionTime << ", delta: " << delta << endl;*/
		Sleep(1000);
	}
}
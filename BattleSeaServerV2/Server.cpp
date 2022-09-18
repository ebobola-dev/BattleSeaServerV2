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
	while (1) {
		printf("hand %u\n", connection.id);
		Sleep(2000);
	}
}

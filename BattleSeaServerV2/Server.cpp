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
		Connection* newConnection = new Connection(client_socket, client_addr);
		printf("New connection! - [%u]\n", newConnection->id);
		connections.push_back(newConnection);
		printf("connections: ");
		for (int i = 0; i < connections.size(); i++) {
			printf("[%d] ", connections[i]->id);
		}
		printf("\n");
		thread newConnectionThread(connectionHandler, this, newConnection);
		newConnectionThread.detach();
	}
}

void Server::close() {
	closesocket(thisSocket);
	WSACleanup();
}

void Server::connectionHandler(Server* s, Connection* conn) {
	thread newConnectionThread(pingHandler, s, conn);
	newConnectionThread.join();
}

void Server::pingHandler(Server* s, Connection* conn) {
	int connIndex = s->getConnectionIndexById(conn->id);
	clock_t pingTime;
	while (conn->isConnected()) {
		try {
			conn->send_(Action::ping);
			pingTime = clock();
		}
		catch (ServerError error) {
			if (error == ServerError::FailedToSend)
				printf("Failed to ping(-1) [%d]\n", conn->id);
			if (error == ServerError::ConnectionClosed)
				printf("Failed to ping(0) [%d]\n", conn->id);
			printf("Connection [%d] closed\n", conn->id);
			s->connections.erase(s->connections.begin() + connIndex);
			return;
		}
		try {
			while (1) {
				Action act = conn->recv_();
				//cout << "recv: " << (int)act << endl;
				if (act == Action::pong) break;
			}
			uint32_t pongTime = (uint32_t)(clock() - pingTime);
			printf("[%d] ping: %d ms\n", conn->id, pongTime);
		}
		catch (ServerError error) {
			if (error == ServerError::FailedToSend)
				printf("Failed to ping(-1)[%d]\n", conn->id);
			if (error == ServerError::ConnectionClosed)
				printf("Failed to ping(0) [%d]\n", conn->id);
			printf("Connection [%d] closed\n", conn->id);
			s->connections.erase(s->connections.begin() + connIndex);
		}
		Sleep(1000);
	}
}

int Server::getConnectionIndexById(uint16_t id) {
	for (int i = 0; i < connections.size(); i++) {
		if (connections[i]->id == id) return i;
	}
	return -1;
}
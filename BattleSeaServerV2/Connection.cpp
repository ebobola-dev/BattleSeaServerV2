#include "Connection.h"

Connection::Connection() : id(0) {
	this->socket_ = INVALID_SOCKET;
	this->connected = false;
}

Connection::Connection(SOCKET socket_, sockaddr_in addr_) : id(addr_.sin_port) {
	this->connected = true;
	this->addr_ = addr_;
	this->socket_ = socket_;
}

bool Connection::isConnected() const { return connected; }

void Connection::send_(Action action, uint8_t* byteArr, size_t arrSize) {
	char* charArr = new char[arrSize + 1];
	charArr[0] = (int)action;
	for (size_t i = 0; i < arrSize; i++) {
		charArr[i + 1] = byteArr[i];
	}
	send(socket_, charArr, arrSize + 1, 0);
}

Action Connection::recv_(uint8_t* byteArr, size_t arrSize) {
	char charArr[MAX_BYTE_ARR_SIZE];
	int8_t bytesRecieved = recv(socket_, &charArr[0], MAX_BYTE_ARR_SIZE, 0);

	if (arrSize > 0) {
		for (size_t i = 0; i < MAX_BYTE_ARR_SIZE; i++)
			byteArr[i] = charArr[i];
	}

	if (charArr[0] < 0 || charArr[0] > MAX_ACTION)
		return Action::unknown;
	return Action(charArr[0]);
}


void Connection::close() {
	this->connected = false;
}
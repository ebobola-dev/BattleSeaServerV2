#include "Connection.h"

Connection::Connection() : id(0) {
	this->socket = INVALID_SOCKET;
}

Connection::Connection(SOCKET socket, sockaddr_in addr) : id(addr.sin_port) {
	this->addr = addr;
	this->socket = socket;
}

void Connection::send_(Action action, uint8_t* byteArr, size_t arrSize) {
	char* charArr = new char[arrSize + 1];
	charArr[0] = (int)action;
	for (size_t i = 0; i < arrSize; i++) {
		charArr[i + 1] = byteArr[i];
	}
	send(socket, charArr, arrSize + 1, 0);
}

Action Connection::recv_(uint8_t* byteArr, size_t arrSize) {
	char charArr[MAX_BYTE_ARR_SIZE];
	recv(socket, &charArr[0], MAX_BYTE_ARR_SIZE, 0);

	for (size_t i = 0; i < MAX_BYTE_ARR_SIZE; i++)
		byteArr[i] = charArr[i];

	if (charArr[0] < 0 || charArr[0] > MAX_ACTION)
		return Action::unknown;
	return Action(charArr[0]);
}
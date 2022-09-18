#include <iostream>
#include <cmath>

#include "Constants.h"
#include "Server.h"
#include "Actions.h"

using namespace std;

void printBinary(uint64_t value) {
	while (value > 0) {
		cout << value % 2;
		value /= 2;
	}
}

int main() {
	Server server(PORT);
	server.start();

	return 0;
}

#include <iostream>
#include <cmath>

#include "Constants.h"
#include "Server.h"
#include "Actions.h"

using namespace std;


int main() {
	Server server(PORT);
	server.start();

	return 0;
}

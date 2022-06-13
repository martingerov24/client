#include "../cppzmq/zmq.hpp"
#include "Client.h"
#include "Server.h"

int main(int argc, char** argv)
{
	if (argc == 1) {
		printf("no argvs passed");
		return -1;
	}
	std::string arg = argv[1];
	Dprintf("arg = %s", arg.c_str());
	if (arg == "--client") {
		Client client;
		client.connect("tcp://127.0.0.1:5555");
		client.sendBuffer("something.txt");
	} else if (arg == "--server"){
		Server server;
		server.bind("tcp://127.0.0.1:5555");
		server.listen();
	} else {
		printf("could not recognize command line argv!\nsupported args are -> \n--client\n--server\n");
		return -1;
	}
	return 0;
}

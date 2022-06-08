#include <iostream>
#include "cppzmq/zmq.hpp"
#include "client.h"

int main()
{
	Client client;
	client.connect("tcp://localhost:5555");
	client.sendBuffer("something.txt");
	return 0;
}
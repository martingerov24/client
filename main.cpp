#include <iostream>
#include "cppzmq/zmq.hpp"
#include "client.h"

int main()
{
	Client client;
	client.connect("tcp://127.0.0.1:5555");
	client.sendBuffer("something.txt"); // the text file is not used for now
	return 0;
}
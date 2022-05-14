#include <iostream>
#include "cppzmq/zmq.hpp"
#include "../client/client.h"

int main()
{
	Client client;
	client.connect("tcp://localhost:5555");
	client.sendCamera(JsonSend::InitImage);// working motherFuckersss

	return 0;
}
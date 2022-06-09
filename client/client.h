#pragma once
#include "../cppzmq/zmq.hpp"
#include "depend.h"
#include "Json.h"

class Client
{
	bool splitAndSendPackage(
		std::vector<int32_t>& package
	);
	bool sendHTTPFlag(
		const HTTPReq request,
		int sizeInBytes
	);
public:
	Client() = default;
	Client(Client& other) = delete;
	Client(Client&& other) = delete;
	Client operator=(Client& other) = delete;
	Client& operator=(Client&& other) = delete;
	void connect(
		const std::string& port
	);
	Result sendBuffer(
		const std::string& filename
	);
	~Client();
private:
	zmq::context_t context{ 1 };
	zmq::socket_t socket{ context, zmq::socket_type::req };
	zmq::message_t message;
};

#pragma once
#include "depend.h"
#include "../cppzmq/zmq.hpp"

class Server
{
	bool catchPostRequest(
	std::vector<int32_t>& tensor,
		const int size
	);
public:
	Server() = default;
	Server(const Server& other) = delete;
	Server(Server&& other) noexcept;
	Server operator=(const Server& other ) = delete;
	void bind(
		const std::string& port
	);
	Server& operator=(
		Server&& other
	) noexcept;
	void listen();
	~Server();
private:
	zmq::context_t context{ 1 };
	zmq::socket_t  socket{ context, zmq::socket_type::rep };
};

#pragma once
#include <thread>
#include <chrono>
#include <conio.h>
#include "../cppzmq/zmq.hpp"
#include "../dependencies/depend.h"

class Client
{
	void Ask();
	Result Stop();
	Result Send(const std::string& message);
	Client(Client& other) = delete;
	Client(Client&& other) = delete;
	Client operator=(Client& other) = delete;
	Client& operator=(Client&& other) = delete;
	bool SplitPackage(const uint8_t* package, int size);
	bool LargeDataTransferInBytes(const std::string& file);
	//Send
	bool readingP1Send(const std::string& fileName, int height, int width);
	bool loadAndSend(const std::string& filename, int& width, int& height);
public:
	bool sendImage(const std::string& imageName);
	Client() = default;
	void connect(const std::string& port);
	Result setClientStatusAndSend(const ClientMsg msg, const SendOptions sendOpt, const std::string& message);
	~Client();
private:
	ClientMsg m_message = ClientMsg::None;
	SendOptions sendOpt = SendOptions::SmallData;
	zmq::context_t context{ 1 };
	zmq::socket_t socket{ context, zmq::socket_type::req };
	zmq::message_t message;
};

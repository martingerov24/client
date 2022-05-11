#include "Client.h"
#include <sys/stat.h>
#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stbi_load.h"
long GetFileSize(const char * filename)
{
	struct stat stat_buf;
	int rc = stat(filename, &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

bool Client::SplitPackage(const uint8_t* package, int size)
{
	//TODO: and a package special number
	int sizeOfBlock = (size + PackageSize - 1) / PackageSize;
	zmq::message_t msg;
	int blockRecursiveSending = 3;
	int test = package[size];
	for (int i = 0; i < sizeOfBlock; i++)
	{
		msg.empty();
		memcpy(msg.data(), &package[i * PackageSize], PackageSize);
		zmq::send_result_t sended = socket.send(msg, zmq::send_flags::sndmore);
		if (!sended.has_value() && blockRecursiveSending >= 0)
		{//find better way
			i--;
			blockRecursiveSending--;
		}
	}
	return blockRecursiveSending >= 1 ? true : false;
}

bool Client::LargeDataTransferInBytes(const std::string& file) //TODO: def thy this
{	//it cannot be nullptr, but lets see if it will crash
	FILE* rdfile = fopen(file.c_str(), "r+");
	std::vector<uint8_t> data;

	if (rdfile == 0) { throw "no file found!"; }
	long size = GetFileSize(file.c_str());
	if (size == -1) { throw "file size is zero!"; }

	data.resize(size);
	fread(reinterpret_cast<char*>(&data[0]), 1, size, rdfile);
	fclose(rdfile);
	return SplitPackage(data.data(), data.size());
}

Result Client::Stop()
{
	return Result::Succeeded; // for now
}

void Client::Ask()
{
}

bool Client::readingP1Send(const std::string &fileName, int height, int width) 
{	//TODO: filename have to contain the heihgt and width
	FILE* rdFile = fopen(fileName.c_str(), "rb+");
	std::vector<uint8_t> data;
	if (rdFile == 0) {
		printf("no file found!");
		return false;
	}
	int size = height * width;
	data.resize(size);
	fread(reinterpret_cast<char*>(&data[0]), 2, size, rdFile);
	fclose(rdFile);
	
	return SplitPackage(data.data(), data.size());
}

bool Client::loadAndSend(const std::string& filename, int& width, int& height)
{
	int channels = 1;
	unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 3);
	//stbir_resize_uint8(img, width, height, 0, img, width, height, 0, 1);
	if (img == NULL) { 
		throw "the file was not found"; 
	}
	uint8_t m = img[1440000];
	std::vector<uint8_t> image(width * height * channels);
	memcpy(&image[0], img, image.size());
	if (image.empty()) { 
		throw "vector is null"; 
	}
	stbi_image_free(img);
	return SplitPackage(image.data(), image.size());
}
bool Client::sendImage(const std::string& imageName)
{
	bool result;
	int width, height, channels;
	if (Client::sendOpt == SendOptions::ImageP1)
	{
		//return readingP1Send(imageName);
	}
	return loadAndSend(imageName, width, height);
}

Result Client::Send(const std::string& msg)
{
	bool result = false;
	if (Client::sendOpt == SendOptions::SmallData)
	{
		socket.send(zmq::buffer(msg), zmq::send_flags::none);
	}
	else if (Client::sendOpt == SendOptions::FileInBytes)
	{
		result = LargeDataTransferInBytes(msg);
	}
	else // else is the image
	{
		result = sendImage(msg); // msg is the file name
	}
	Client::m_message = ClientMsg::None;
	return result == true ? Result::Succeeded : Result::FailedToSend;
}


Result Client::setClientStatusAndSend(const ClientMsg msg = ClientMsg::None, const SendOptions sendOpt = SendOptions::SmallData, const std::string & message = "")
{
	Result temp;
	this->sendOpt = sendOpt;
	if (msg == ClientMsg::Send)
	{
		temp = Send(message);
	}
	else if (msg == ClientMsg::Stop)
	{
		
	}
	return Result::Succeeded;
}
void Client::connect(const std::string& port)
{
	socket.connect(port);
};

Client::~Client()
{
	message.empty();
	socket.close();
	context.close();
}

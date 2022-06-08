#include "client.h"
#include <sys/stat.h>
#include <fstream>
#define STB_IMAGE_IMPLEMENTATION
#include "../dependencies/stbi_load.h"

//not used
long GetFileSize(
    const char * filename
)
{
	struct stat stat_buf;
	int rc = stat(filename, &stat_buf);
	return rc == 0 ? stat_buf.st_size : -1;
}

void Client::connect(
    const std::string& port
)
{
	socket.connect(port);
};

Client::~Client()
{
	message.empty();
	socket.close();
	context.close();
}

bool readImage(
    const std::string& filename,
    std::vector<uint8_t>& output
)
{
    int width, height, channels;
    unsigned char* img = stbi_load(filename.c_str(), &width, &height, &channels, 1);
	//stbir_resize_uint8(img, width, height, 0, img, width, height, 0, 1);
	if (img == NULL) 
    { 
        return false;
    }
	output.resize(width * height); // for now working with 1 channelled image
	memcpy(&output[0], img, output.size());
	if (output.empty())
    {
        return false;
    }
	stbi_image_free(img);
	return true;
}

bool readingFiles(
    const std::string& fileName,
    std::vector<uint8_t>& output
)
{
	FILE* rdFile = fopen(fileName.c_str(), "rb+");
	if (rdFile == 0) {
		printf("no file found!");
		return false; 
	}
    //TODO:: this solution is temporary, because i could not find a better one
	int size = GetFileSize(fileName.c_str());
	output.resize(size);
	fread(reinterpret_cast<char*>(&output[0]), 2, size, rdFile);
	fclose(rdFile);
    if(output.empty())
    {
        printf("file has no contents!");
        return true;
    }
	return true;
}

bool Client::splitAndSendPackage(
    std::vector<uint8_t>& package
)
{
	//TODO: and a package special number
	int totalPackagesToSend = (package.size() + PackageSize - 1) / PackageSize;
	for (int i = 0; i < totalPackagesToSend ; i++)
	{
	    zmq::message_t msg(PackageSize);
		memcpy(msg.data(), &package[i * PackageSize], PackageSize);
		zmq::send_result_t sended = socket.send(msg.data(), msg.size(), ZMQ_DONTWAIT);
	}
    return true;
}
Result Client::sendHTTPFlag(const HTTPReq request)
{
    
}

Result Client::sendBuffer(const std::string& filename,
    bool isPhoto
)
{
    std::vector<uint8_t> output;
    bool succeededReading;
    if(isPhoto)
    {
        succeededReading = readImage(filename, output);
    }
    else{
        succeededReading = readingFiles(filename, output);
    }

    //Send funciton here


    return Result::Succeeded;
}

Result Client::sendCamera(
    JsonSend sendOpt
)
{
    std::string send = json.jsonShare(sendOpt);

    zmq::message_t query(send.length());
    memcpy(query.data(), (send.c_str()), (send.size()));
    socket.send(query);

    zmq::message_t reply;
    socket.recv(reply);
    auto reply_str = std::string(static_cast<char*>(reply.data()), reply.size());
    // std::cout << reply_str << std::endl;
	
	if (reply_str.empty())
		return Result::FailedToSend;
    return Result::Succeeded;
}

#include "client.h"
#include <sys/stat.h>
#include <fstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stbi_load.h"

int32_t GetFileSize(
	const char* filename
) {
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
    if(filename == nullptr)
    {
        return -1;
    }
    FILE *p_file = fopen(filename,"rb");;
    if(p_file == nullptr)
    {
        return -2;
    }
    fseek(p_file,0,SEEK_END);
    int32_t size = ftell(p_file);
    fclose(p_file);
    return size;
}


void Client::connect(
    const std::string& port
) {
	socket.connect(port);
};

Client::~Client() {
	message.empty();
	socket.close();
	context.close();
}

bool readingFiles(
    const std::string& fileName,
    std::vector<int32_t>& output
) {
	FILE* rdFile = fopen(fileName.c_str(), "rb+");
	if (rdFile == 0) {
		printf("no file found!");
		return false;
	}
	int32_t size = GetFileSize(fileName.c_str());
	if (size <=0 ) {
		return false;
	}
	output.resize(size);
	fread(reinterpret_cast<char*>(&output[0]), sizeof(int32_t), size, rdFile);
	fclose(rdFile);
    if(output.empty()) {
        printf("file has no contents!");
        return true;
    }
	return true;
}

bool Client::splitAndSendPackage(
    std::vector<int32_t>& package
) {
	//TODO: and a package special number
	int totalPackagesToSend = (package.size() + PackageSize - 1) / PackageSize;
	for (int i = 0; i < totalPackagesToSend ; i++) {
	    zmq::message_t msg(PackageSize);
		memcpy(msg.data(), &package[i * PackageSize], PackageSize);
		zmq::send_result_t sended = socket.send(msg.data(), msg.size(), ZMQ_DONTWAIT);
	}
    return true;
}

Result Client::sendHTTPFlag(
	const HTTPReq request,
	int sizeInBytes
) {
	std::string sendMsg = JsonFile::prepareHTTPReqPost(request, sizeInBytes);
	int rc = socket.send(sendMsg.data(), sendMsg.size());
	if (rc < 0) {
		Result::FailedToSend;
	}
	return Result::Succeeded;
}

Result Client::sendBuffer(
	const std::string& filename
) {
    std::vector<int32_t> output;
    // bool succeededReading = readingFiles(filename, output);
	Result res = sendHTTPFlag(HTTPReq::Post, 2000);
	if (res == Result::FailedToSend){
		printf("could not send Http flag\n");
		Result::FailedToSend;
	}
    return Result::Succeeded;
}

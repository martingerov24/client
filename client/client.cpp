#include "client.h"
#include <sys/stat.h>
#include <fstream>

int32_t GetFileSize(
	const char* filename
) {
    struct stat stat_buf;
    int rc = stat(filename, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
    if(filename == nullptr) {
        return -1;
    }
    FILE *p_file = fopen(filename,"rb");
    if(p_file == nullptr) {
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
	socket.close();
	context.close();
}

bool readingFiles(
    const std::string& fileName,
    std::vector<int32_t>& output
) {
	FILE* rdFile = fopen(fileName.c_str(), "rb+");
	if (rdFile == 0) {
		Dprintf("%d file not found", 0);
		return false;
	}
	int32_t size = GetFileSize(fileName.c_str());
	if (size <=0 ) {
		Dprintf("%d size of file is below zero", size);
		return false;
	}
	output.resize(size);
	fread(reinterpret_cast<char*>(&output[0]), sizeof(int32_t), size, rdFile);
	fclose(rdFile);
    if(output.empty()) {
        Dprintf("file has no contents %d", 0);
        return true;
    }
	return true;
}

bool Client::splitAndSendPackage(
    std::vector<int32_t>& package
) {
	//TODO: add package special number
	int totalPackagesToSend = (package.size() + PackageSize - 1) / PackageSize;
	Dprintf("%d -> iterations", totalPackagesToSend);
	socket.send(zmq::buffer(package.data(), package.size() * sizeof(int32_t)),zmq::send_flags::none);
	zmq::message_t msg;
	socket.recv(msg, zmq::recv_flags::none);
	if(msg.data() == nullptr){
		Dprintf("did not recieve any message %d",0);
		return false;
	}
	Dprintf("%s", msg.to_string().c_str());
    return true;
}

bool Client::sendHTTPFlag(
	const HTTPReq request,
	int sizeInBytes
) {
	std::string sendMsg = JsonFile::prepareHTTPReqPost(request, sizeInBytes);
	socket.send(sendMsg.data(), sendMsg.size());
	//TODO: see how to check
	return true;
}

Result Client::sendBuffer(
	const std::string& filename
) {
    std::vector<int32_t> output(200*100, 1);
	Dprintf("the output size is --> %d", output.size()*sizeof(int32_t));

	bool succeeded = sendHTTPFlag(
		HTTPReq::Post,
		output.size() * sizeof(output[0])
	);
	if (succeeded == false) {
		Dprintf("%d could not send Http flag", 0);
		return Result::FailedToSend;
	}

	zmq::message_t msg;
	socket.recv(msg); // -> see if there is still a connection
	Dprintf("%s -> recv messsage", msg.to_string().c_str());

	succeeded = splitAndSendPackage(output);
    if (succeeded == false) {
		Dprintf("%d -> could not split and send package", 0);
		return Result::FailedToSend;
	}
	return Result::Succeeded;
}

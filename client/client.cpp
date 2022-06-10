#include "client.h"
#include <sys/stat.h>
#include <fstream>

//do not need them for now
// 		#define STB_IMAGE_IMPLEMENTATION
// 		#include "stbi_load.h"

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

void myFreeFunc(void* arr, void* thing)
{
	if (arr == nullptr){
		return;
	}

	delete [] reinterpret_cast<int*>(arr);
}

bool Client::splitAndSendPackage(
    std::vector<int32_t>& package
) {
	//TODO: add package special number
	int totalPackagesToSend = (package.size() + PackageSize - 1) / PackageSize;
	int* arr = new int[PackageSize];
	// zmq::message_t msg(&arr, PackageSizeInBytes, &myFreeFunc, nullptr);
	zmq_msg_t msg;
	zmq_msg_init(&msg);
	zmq_msg_init_data(&msg, &arr, PackageSizeInBytes, myFreeFunc, nullptr);
	Dprintf("%d -> iterations", totalPackagesToSend);
	int retValue = zmq_send(&socket, zmq_msg_data(&msg), zmq_msg_size(&msg), ZMQ_SNDMORE);
	Dprintf("%d -> ret value", retValue);
	// for (int i = 0; i < totalPackagesToSend ; i++) {
	// 	// zmq::send_result_t sended = socket.send(
	// 	// 	package[i*PackageSize],
	// 	// 	PackageSizeInBytes,
	// 	// 	ZMQ_DONTWAIT
	// 	// );
	//     zmq::message_t msg(PackageSizeInBytes);
	// 	memcpy(msg.data(), &package, PackageSizeInBytes+1);

	// 	int retValue = zmq_send(&socket, &msg, PackageSizeInBytes+1, ZMQ_NOBLOCK);
	// 	// zmq::send_result_t sended = socket.send(msg.data(), msg.size()*sizeof(int32_t), ZMQ_DONTWAIT);
	// 	Dprintf("%d end funciton returned", retValue);
	// 	zmq_sleep(1);
	// 	socket.recv(&msg);
	// 	Dprintf("msg in send loop received %s", msg.to_string().c_str());
	// }
    return true;
}

bool Client::sendHTTPFlag(
	const HTTPReq request,
	int sizeInBytes
) {
	std::string sendMsg = JsonFile::prepareHTTPReqPost(request, sizeInBytes);
	int rc = socket.send(sendMsg.data(), sendMsg.size());
	if (rc < 0) {
		Dprintf("socket send returned -> %d", rc);
		return false;
	}
	return true;
}

Result Client::sendBuffer(
	const std::string& filename
) {
    std::vector<int32_t> output(200*100, 1);
	Dprintf("the output size is --> %d", output.size()*sizeof(int32_t));
    // bool succeeded = readingFiles(filename, output);
	// if (succeeded == false) {
	// 	printf("could not read the file!\n");
	// 	return Result::FailedToSend;
	// }

	bool succeeded = sendHTTPFlag(
		HTTPReq::Post,
		output.size() * sizeof(output[0])
	);
	if (succeeded == false) {
		Dprintf("%d could not send Http flag", 0);
		Result::FailedToSend;
	}

	zmq::message_t msg;
	socket.recv(msg); // test method, to see if there is still a connection
	Dprintf("%s -> recv messsage", msg.to_string().c_str());

	succeeded = splitAndSendPackage(output);
    if (succeeded == false) {
		Dprintf("%d -> could not split and send package", 0);
		return Result::FailedToSend;
	}
	return Result::Succeeded;
}

#include "Json.h"

#include "json.hpp"
// #include <iostream>
std::string JsonFile::prepareHTTPReqPost(HTTPReq request, int sizeToSendInBytes) {
	//explicitly create a json file, but soon or later imma remove this
	nlohmann::json json;
	json["Post"] = sizeToSendInBytes;
	// std::cout << json.dump();
	return json.dump();
}
int JsonFile::getReqAndSize(
	const std::string& msg,
	HTTPReq& req
) {
	if (msg.empty())
	{
		printf("the recieve msg is empty!\n");
		return -1;
	}
	nlohmann::json json = nlohmann::json::parse(msg);
	if (json.contains("Post") == false){
		printf("cannot handle different request types in this funciton\n");
		return -1;
	}
	req = HTTPReq::Post;
	return json.value("Post", 0);
}

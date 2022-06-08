#pragma once 
#include "../dependencies/json.hpp"
#include "../dependencies/depend.h"
using json = nlohmann::json;
struct JsonFile
{
	// in reserve i have added 5 jsons
	JsonFile() = default;
	JsonFile(HTTPReq request, int32_t sizeOfFile);
	std::string jsonShare(JsonSend json);
	~JsonFile() = default;
private:

	std::vector<nlohmann::json> premadeJsons =
	{
		{"Get", 0},
		{"Post", 0},
		{"Shotwdown", 0}
	};
};
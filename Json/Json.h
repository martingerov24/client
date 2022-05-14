#pragma once 
#include "../dependencies/json.hpp"
#include "../dependencies/depend.h"

struct JsonFile
{
	// in reserve i have added 5 jsons
	JsonFile() = default;
	std::string jsonShare(JsonSend json);
	~JsonFile() = default;
private:
	std::vector<nlohmann::json> premadeJsons = {
	//initCamera
	{
	  {"width", 800},
	  {"height", 600},
	  {"samplesPerPixel", 4}
	},
	//cameralookAt
	{
	  {"verticalFov", 90},
	  {"lookFrom", {-1,5,-1}},
	  {"lookAt", {0,0,0}}
	} };
};
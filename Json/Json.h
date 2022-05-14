#pragma once 
#include "../dependencies/json.hpp"
#include "../dependencies/depend.h"
using json = nlohmann::json;
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
	  {"verticalFov",90},
	  {"lookFromX", -1},
	  {"lookFromY",  5},
	  {"lookFromZ", -1},
	  {"lookAtX", 0},
	  {"lookAtY", 0},
	  {"lookAtZ", 0}
	} };
};
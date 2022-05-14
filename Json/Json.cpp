#include "Json.h"


std::string JsonFile::jsonShare(JsonSend json)
{
	return premadeJsons[(int)json].dump();
}

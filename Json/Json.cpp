#include "Json.h"

JsonFile::JsonFile(HTTPReq request, int32_t sizeOfFile)
{
	premadeJsons[static_cast<int>(request)] = sizeOfFile;
}

std::string JsonFile::jsonShare(JsonSend json)
{
	return premadeJsons[(int)json].dump();
}

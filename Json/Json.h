#pragma once
#include "depend.h"
#include <string>
namespace JsonFile
{
	std::string prepareHTTPReqPost(
		HTTPReq request,
		int sizeToSendInBytes
	);
}

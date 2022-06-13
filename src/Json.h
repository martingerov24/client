#pragma once
#include "depend.h"
#include <string>
namespace JsonFile
{
	int getReqAndSize(
        const std::string& msg,
        HTTPReq& req
    );
	std::string prepareHTTPReqPost(
		HTTPReq request,
		int sizeToSendInBytes
	);
}

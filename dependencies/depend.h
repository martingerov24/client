#pragma once
#include <stdint.h>
struct float3
{
	float x;
	float y;
	float z;
};

struct float2
{
	float x;
	float y;
};

enum class HTTPReq : uint8_t
{
	Get,
	Post,
	Shutdown
};

enum class Result : uint8_t
{
	FailedToSend = 0,
	Succeeded = 1,
};

const int PackageSize = 2000;

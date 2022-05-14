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
enum class JsonSend : uint8_t
{
	InitImage = 0,
	CameraLookAt = 1
};
const int PackageSize = 5000;
//int ClientCount = 0;
enum class ClientMsg : uint8_t
{
	Send,
	Stop,
	Deactivate,
	None
};
enum class SendOptions : uint8_t
{
	SmallData,
	FileInBytes,
	Image,
	ImageP1,
	Count
};
enum class Result : uint8_t
{
	FailedToSend = 0,
	Succeeded = 1,
};
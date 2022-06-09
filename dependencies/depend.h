#pragma once
#include <stdint.h>

#ifdef NDEBUG
#define Dprintf(FORMAT, ...) ((void)0)
#define DebugPuts(MSG) ((void)0)
#else
#define Dprintf(FORMAT, ...) \
    fprintf(stderr, "----- Debug  func -> %s() in %s, line %i: " FORMAT "\n", \
        __func__, __FILE__, __LINE__, __VA_ARGS__)
#define DebugPuts(MSG) Dprintf("%s", MSG)
#endif

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
const int PackageSizeInBytes = PackageSize*sizeof(int);

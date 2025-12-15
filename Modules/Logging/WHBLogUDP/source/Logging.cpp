#include "TeaPacket/Logging/Logging.hpp"

#include <whb/log_udp.h>
#include <whb/log_cafe.h>
#include <whb/log.h>

void TeaPacket::Logging::Initialize()
{
    WHBLogUdpInit();
    WHBLogCafeInit();
}
void TeaPacket::Logging::DeInitialize()
{
    WHBLogUdpDeinit();
    WHBLogCafeDeinit();
}


void TeaPacket::LogString(const std::string& msg)
{
    WHBLogPrintf(msg.c_str());
    //WHBLogPrintf("\n");
}

#include "TeaPacket/Logging/Logging.hpp"

#include <whb/log_udp.h>

void TeaPacket::Logging::Initialize()
{
    WHBLogUdpInit();
}
void TeaPacket::Logging::DeInitialize()
{
    WHBLogUdpDeinit();
}


void TeaPacket::LogString(const std::string& msg)
{
    OSReport(msg.c_str());
    OSReport("\n");
}

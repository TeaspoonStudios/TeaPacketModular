#include "TeaPacket/Logging/Logging.hpp"

#include <iostream>

void TeaPacket::Logging::Initialize() {}
void TeaPacket::Logging::DeInitialize() {}


void TeaPacket::LogString(const std::string& msg)
{
    std::cout << msg << "\n";
}

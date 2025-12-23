#pragma once
#include "TeaPacket/Core/Core.hpp"

namespace TeaPacket::Input
{
    void Initialize();
    void DeInitialize();
    
}
TP_HookInitDeInitFunctions(Input, 5);
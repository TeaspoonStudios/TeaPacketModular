#pragma once

#include "GraphicsHeap/DisposableForegroundResource.hpp"

namespace TeaPacket::Graphics
{
    enum class GX2Display : bool
    {
        TV = false,
        DRC = true,
    };
    
    struct PlatformDisplay
    {
        GX2::DisposableForegroundMemResource scanBuffer;
        
        GX2Display displayType : 1;
    };
}

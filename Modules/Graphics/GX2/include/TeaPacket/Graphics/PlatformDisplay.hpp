#pragma once

#include "GraphicsHeap/DisposableForegroundResource.hpp"
#include "GraphicsHeap/MEM2Resource.hpp"

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

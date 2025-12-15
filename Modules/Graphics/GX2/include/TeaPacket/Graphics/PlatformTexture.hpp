#pragma once

#include <gx2/texture.h>

namespace TeaPacket::Graphics
{
    
    struct PlatformTexture
    {
        GX2Texture gx2Texture;

        bool isPartOfViewport : 1 = false;
    };
}

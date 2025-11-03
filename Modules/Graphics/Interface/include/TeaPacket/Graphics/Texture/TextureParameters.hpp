#pragma once

#include <cstdint>

namespace TeaPacket::Graphics
{
    enum class TextureFormat : unsigned short;

    enum class TextureAvailableMode : unsigned char{
        None,
        GPU,
        CPU,
    };

    struct TextureUseFlags
    {
        bool shaderResource : 1 = true;
        bool renderTargetColor : 1 = false;
        bool renderTargetDepth : 1 = false;
        TextureAvailableMode writeMode : 2 = TextureAvailableMode::None;
        bool cpuReadable : 1 = false;
    };

    struct TextureParameters
    {
        void* data;
        uint16_t width;
        uint16_t height;
        TextureFormat format;
        TextureUseFlags useFlags;
    };
}

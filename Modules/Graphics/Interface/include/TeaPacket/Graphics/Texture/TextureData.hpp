#pragma once

#include "TeaPacket/Types/Memory/FixedArray.hpp"
#include "TeaPacket/Math/Color.hpp"
#include "TeaPacket/Graphics/Texture/TextureFormat.hpp"

namespace TeaPacket::Graphics
{

    class TextureData
    {
    public:
        FixedArray<unsigned char> data;
        uint16_t pitch = 0;

        uint16_t width = 0;
        uint16_t height = 0;

        TextureFormat format;

        explicit TextureData(const size_t dataSize) :
            data(dataSize), format()
        {
        }

        Color GetColor4(const uint16_t x, const uint16_t y)
        {
            unsigned char* p = &data[y * pitch + x * GetTextureFormatBytesPerPixel(format)];
            switch (format)
            {
                using enum TextureFormat;
            case RGBA8:
                return Color{*p, p[1], p[2], p[3]};
            case BGRA8:
                return Color{p[2], p[1], *p, p[3]};
            default: throw std::exception();
            }
        }

    };
}
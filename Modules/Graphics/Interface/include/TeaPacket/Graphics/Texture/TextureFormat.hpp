#pragma once

#include <vector>
#include <cstdint>

namespace TeaPacket::Graphics
{
    /// A format for a texture. Determines how the data is interpreted on creation, and how much space the format takes up on the GPU.
    enum class TextureFormat : unsigned short
    {
        RGBA8, ///< A 4 channel 32 bpp texture format that supports 8 bits for Red, Green, Blue, Alpha
        BGRA8, ///< A 4 channel 32 bpp texture format that supports 8 bits for Blue, Green, Red, Alpha
        D24S8, ///< A 32 bpp texture for 24 bits of depth and 8 bits of mask
        NONE
    };

    /// Returns the size of each channel of a format.
    [[nodiscard]] constexpr std::vector<unsigned char> GetTextureFormatChannelSizes(const TextureFormat format){
        using enum TextureFormat;
        switch (format)
        {
        case BGRA8:
        case RGBA8: return {8,8,8,8};
        case D24S8: return {24, 8};
        case NONE: throw std::exception();
        }
        throw std::exception();
    }

    /// Returns the Bits-Per-Pixel of a texture format.
    [[nodiscard]] constexpr  uint8_t GetTextureFormatBPP(const TextureFormat format)
    {
        using enum TextureFormat;
        switch (format)
        {
        case BGRA8:
        case RGBA8:
        case D24S8: return 32;
        case NONE: throw std::exception();
        }
        throw std::exception();
    }

    /// Returns the Bytes-Per-Pixel of a texture format.
    [[nodiscard]] constexpr uint8_t GetTextureFormatBytesPerPixel(const TextureFormat format)
    {
        return GetTextureFormatBPP(format)/8;
    }
}
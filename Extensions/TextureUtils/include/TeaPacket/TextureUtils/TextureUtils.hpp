#pragma once

#include "TeaPacket/Math/Color.hpp"
#include "TeaPacket/Graphics/Texture/TextureFormat.hpp"
#include "TeaPacket/Types/Memory/FixedArray.hpp"

#define TP_TexUtilsParams \
FixedArray<unsigned char>& texData, \
unsigned int x, unsigned int y, \
unsigned int pitch

namespace TeaPacket::TextureUtils
{
    template<Graphics::TextureFormat format, typename retType>
    retType GetPixel(TP_TexUtilsParams) = delete;

    template<>
    Color GetPixel<Graphics::TextureFormat::RGBA8>(TP_TexUtilsParams);

    template<>
    Color GetPixel<Graphics::TextureFormat::BGRA8>(TP_TexUtilsParams);

}
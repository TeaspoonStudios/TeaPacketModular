#include "TeaPacket/Graphics/Texture/Texture.hpp"
#include "TeaPacket/Graphics/PlatformTexture.hpp"

#include <gx2/texture.h>
#include <gx2/mem.h>
#include <gx2/utils.h>

#include "TeaPacket/Graphics/Texture/TextureParameters.hpp"

using namespace TeaPacket::Graphics;

Texture::Texture(const TextureParameters& parameters):
platformTexture(std::make_unique<PlatformTexture>(PlatformTexture{
    .gx2Texture = GX2Texture{
        .surface = GX2Surface{
            .dim = GX2_SURFACE_DIM_TEXTURE_2D,
            .width = parameters.width,
            .height = parameters.height,
            .depth = 1,
            .mipLevels = 1,
            .format = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
            .aa = GX2_AA_MODE1X,
            .use = GX2_SURFACE_USE_NONE,
            .imageSize = 0,
            .image = nullptr,
            .mipmapSize = 0,
            .mipmaps = nullptr,
            .tileMode = GX2_TILE_MODE_DEFAULT,
            .swizzle = 0,
            .alignment = 0,
            .pitch = 0,
            .mipLevelOffset = {}
        },
        .viewFirstMip = 0,
        .viewNumMips = 1,
        .viewFirstSlice = 0,
        .viewNumSlices = 1,
        .compMap = GX2_COMP_MAP(GX2_SQ_SEL_R, GX2_SQ_SEL_G, GX2_SQ_SEL_B, GX2_SQ_SEL_A),
        .regs = {}
    },

    .isPartOfViewport = parameters.useFlags.renderTargetColor || parameters.useFlags.renderTargetDepth
})),
width(parameters.width),
height(parameters.height),
format(parameters.format)
{
    if (platformTexture->isPartOfViewport)
    {
        return; // This texture is purely dummy
    }
}

Texture::~Texture() = default;

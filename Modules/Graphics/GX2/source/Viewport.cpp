#include "TeaPacket/Graphics/Viewport.hpp"

#include <gx2/clear.h>
#include <gx2/context.h>
#include <gx2/mem.h>
#include <gx2/registers.h>
#include <gx2/swap.h>

#include "TeaPacket/Graphics/PlatformDisplay.hpp"
#include "GraphicsHeap/GraphicsHeap.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/PlatformViewport.hpp"

#include "TeaPacket/Graphics/ViewportParameters.hpp"
#include "TeaPacket/Graphics/Texture/Texture.hpp"
#include "TeaPacket/Graphics/Texture/TextureFormat.hpp"
#include "TeaPacket/Graphics/Texture/TextureParameters.hpp"

using namespace TeaPacket::Graphics;
using namespace TeaPacket::GX2;

static void AllocateGX2Invalidate(const DisposableForegroundMemResource* resource)
{
    GX2Invalidate(
        GX2_INVALIDATE_MODE_CPU,
        resource->data,
        resource->GetSize());
}

// WHAT IS THIS BEHEMOTH OF AN INITIALIZER LIST
Viewport::Viewport(const ViewportParameters& parameters):
platformViewport(std::make_unique<PlatformViewport>(PlatformViewport{
    .colorBuffer = {
        .surface = GX2Surface{
            .dim = GX2_SURFACE_DIM_TEXTURE_2D,
            .width = parameters.width,
            .height = parameters.height,
            .depth = 1,
            .mipLevels = 1,
            .format = GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
            .aa = GX2_AA_MODE1X,
            .use = GX2_SURFACE_USE_TEXTURE_COLOR_BUFFER_TV,
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
        .viewMip = 0,
        .viewFirstSlice = 0,
        .viewNumSlices = 1,
        .aaBuffer = nullptr,
        .aaSize = 0,
        .regs = {}
    },
    .colorBufferImage = DisposableForegroundMemResource(),
    .depthBuffer = {
        .surface = GX2Surface{
            .dim = GX2_SURFACE_DIM_TEXTURE_2D,
            .width = parameters.width,
            .height = parameters.height,
            .depth = 1,
            .mipLevels = 1,
            .format = GX2_SURFACE_FORMAT_FLOAT_R32,
            .aa = GX2_AA_MODE1X,
            .use = GX2_SURFACE_USE_DEPTH_BUFFER,
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
        .viewMip = 0,
        .viewFirstSlice = 0,
        .viewNumSlices = 1,
        .hiZPtr = nullptr,
        .hiZSize = 0,
        .depthClear = 1.0f,
        .stencilClear = 0,
        .regs = {}
    },
    .depthBufferImage = DisposableForegroundMemResource(),
})),

colorTexture(TextureParameters{
    .data = nullptr,
    .width = parameters.width,
    .height = parameters.height,
    .format = TextureFormat::RGBA8,
    .useFlags = {
        .shaderResource = parameters.flags.shaderUsable,
        .renderTargetColor = true,
        .renderTargetDepth = false,
        .writeMode = TextureAvailableMode::GPU,
        .cpuReadable = true,
    },
    .filterMode = TextureFilterMode::Linear,
    .wrapMode = TextureWrapMode::Wrap
}),
depthTexture(TextureParameters{
    .data = nullptr,
    .width = parameters.width,
    .height = parameters.height,
    .format = TextureFormat::D24S8,
    .useFlags = {
        .shaderResource = parameters.flags.shaderUsable,
        .renderTargetColor = false,
        .renderTargetDepth = true,
        .writeMode = TextureAvailableMode::GPU,
        .cpuReadable = true
    },
    .filterMode = TextureFilterMode::Linear,
    .wrapMode = TextureWrapMode::Wrap
}),
ownedDisplay(parameters.ownedDisplay)
{
    GX2CalcSurfaceSizeAndAlignment(&platformViewport->colorBuffer.surface);
    GX2InitColorBufferRegs(&platformViewport->colorBuffer);

    GX2CalcSurfaceSizeAndAlignment(&platformViewport->depthBuffer.surface);
    GX2InitDepthBufferRegs(&platformViewport->depthBuffer);

    // Alloc color buffer
    if (!platformViewport->colorBufferImage.Initialize(
        ForegroundBucket::MEM1,
        platformViewport->colorBuffer.surface.imageSize,
        static_cast<int>(platformViewport->colorBuffer.surface.alignment),
        AllocateGX2Invalidate
    ))
    {
        throw std::runtime_error("failed to allocate color buffer");
    }
    platformViewport->colorBuffer.surface.image = platformViewport->colorBufferImage.data;
    

    // Alloc Depth Buffer
    if (!platformViewport->depthBufferImage.Initialize(
        ForegroundBucket::MEM1,
        platformViewport->depthBuffer.surface.imageSize,
        static_cast<int>(platformViewport->depthBuffer.surface.alignment),
        AllocateGX2Invalidate
    ))
    {
        throw std::runtime_error("failed to allocate depth buffer");
    }
    platformViewport->depthBuffer.surface.image = platformViewport->depthBufferImage.data;
}

void Viewport::BeginRender()
{
    GX2SetColorBuffer(&platformViewport->colorBuffer, GX2_RENDER_TARGET_0);
    GX2SetDepthBuffer(&platformViewport->depthBuffer);
    GX2SetViewport(0, 0,GetWidth(),GetHeight(),0.0f, 1.0f);
    GX2SetScissor(0, 0, GetWidth(),GetHeight());
    activeViewport = this;
}

void Viewport::FinishRender()
{
    if (ownedDisplay)
    {
        const GX2ScanTarget target =
            ownedDisplay->platformDisplay->displayType == GX2Display::TV ?
            GX2_SCAN_TARGET_TV : GX2_SCAN_TARGET_DRC;
        GX2CopyColorBufferToScanBuffer(&platformViewport->colorBuffer, target);
    }
}

constexpr float CharTo1Factor = 1.0f/255.0f;

void Viewport::ClearColor(const uint8_t r, const uint8_t g, const uint8_t b)
{
    if (activeViewport == nullptr) { return; }
    GX2ClearColor(&activeViewport->platformViewport->colorBuffer,
        r * CharTo1Factor, g * CharTo1Factor, b * CharTo1Factor, 1.0f);
    GX2ClearDepthStencilEx(&activeViewport->platformViewport->depthBuffer,
        activeViewport->platformViewport->depthBuffer.depthClear,
        activeViewport->platformViewport->depthBuffer.stencilClear,
        GX2_CLEAR_FLAGS_DEPTH | GX2_CLEAR_FLAGS_STENCIL);
}



Viewport::~Viewport() = default;

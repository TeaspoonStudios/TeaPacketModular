#include "TeaPacket/Graphics/Display.hpp"

#include <gx2/context.h>
#include <gx2/display.h>
#include <gx2/event.h>
#include <gx2/mem.h>
#include <gx2/registers.h>
#include <gx2/state.h>
#include <gx2/swap.h>

#include "GraphicsHeap/DisposableForegroundResource.hpp"
#include "GraphicsHeap/GraphicsHeap.hpp"
#include "TeaPacket/Graphics/PlatformDisplay.hpp"

#include "TeaPacket/Graphics/Viewport.hpp"
#include "TeaPacket/Graphics/PlatformViewport.hpp"

#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/ViewportParameters.hpp"

using namespace TeaPacket::Graphics;
using namespace TeaPacket::GX2;

static void AllocateTVScanBuffer(const DisposableForegroundMemResource* scanBuffer)
{
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU, scanBuffer->data, scanBuffer->GetSize());
    GX2SetTVBuffer(
        scanBuffer->data,
        scanBuffer->GetSize(),
        GX2_TV_RENDER_MODE_WIDE_720P,
        GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
        GX2_BUFFERING_MODE_DOUBLE);
}
static void AllocateDRCScanBuffer(const DisposableForegroundMemResource* scanBuffer)
{
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU, scanBuffer->data, scanBuffer->GetSize());
    GX2SetDRCBuffer(
        scanBuffer->data,
        scanBuffer->GetSize(),
        GX2_DRC_RENDER_MODE_DOUBLE,
        GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
        GX2_BUFFERING_MODE_DOUBLE);
}

Display::Display(const DisplayParameters& parameters):
platformDisplay(std::make_unique<PlatformDisplay>(PlatformDisplay{
    .scanBuffer = DisposableForegroundMemResource(),
    .displayType = static_cast<GX2Display>(GetDisplayCount() == 0),
})),
viewport(ViewportParameters{
    .width = parameters.width,
    .height = parameters.height,
    .flags = {
        .shaderUsable = false
    },
    .ownedDisplay = this
    })
{
    uint32_t size;
    uint32_t _dump;
    const bool isTV = GetDisplayCount() == 0;
    if (isTV)
    {
        // TV
        GX2CalcTVSize(
            GX2_TV_RENDER_MODE_WIDE_720P,
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
            GX2_BUFFERING_MODE_DOUBLE,
            &size,
            &_dump);

        //HeapAllocForeground(size, GX2_SCAN_BUFFER_ALIGNMENT);
        if (!platformDisplay->scanBuffer.Initialize(
            ForegroundBucket::Foreground,
            size,
            GX2_SCAN_BUFFER_ALIGNMENT,
            AllocateTVScanBuffer
        ))
        {
            throw std::runtime_error("COULDN'T ALLOC TV SCAN BUFFER");
        }
        GX2SetTVScale(parameters.width, parameters.height);
    } else
    {
        // DRC
        const GX2DrcRenderMode drcRenderMode = GX2GetSystemDRCMode();
        GX2CalcDRCSize(
            drcRenderMode,
            GX2_SURFACE_FORMAT_UNORM_R8_G8_B8_A8,
            GX2_BUFFERING_MODE_DOUBLE,
            &size,
            &_dump);

        if (!platformDisplay->scanBuffer.Initialize(
            ForegroundBucket::Foreground,
            size,
            GX2_SCAN_BUFFER_ALIGNMENT,
            AllocateDRCScanBuffer
        ))
        {
            throw std::runtime_error("COULDN'T ALLOC DRC SCAN BUFFER");
        }
        GX2SetDRCScale(parameters.width, parameters.height);
    }
    GX2SetSwapInterval(1);
}

void Display::InitializeDefaultDisplays(const std::vector<DisplayParameters>& requestedParameters)
{
    // TODO: RESPECT REQUESTED PARAMS
    (void)requestedParameters;
    Displays.emplace_back(std::make_unique<Display>(DisplayParameters{
        .width = 1920,
        .height = 1080
    }));

    Displays.emplace_back(std::make_unique<Display>(DisplayParameters{
        .width = 854,
        .height = 480
    }));
}

Display::~Display() = default;

void Display::DeInitialize()
{
    Displays.clear();
}

void Display::WaitForVSync()
{
    uint32_t swapCount, flipCount;
    OSTime lastFlip, lastVsync;
    uint32_t waitCount = 0;

    while (true) {
        GX2GetSwapStatus(&swapCount, &flipCount, &lastFlip, &lastVsync);

        if (flipCount >= swapCount) {
            break;
        }

        if (waitCount >= 10) {
            break;
        }

        waitCount++;
        GX2WaitForVsync();
    }
}


void Display::PresentAll()
{
    GX2SwapScanBuffers();
    GX2Flush();
    GX2DrawDone();
    GX2SetTVEnable(TRUE);
    GX2SetDRCEnable(TRUE);
}

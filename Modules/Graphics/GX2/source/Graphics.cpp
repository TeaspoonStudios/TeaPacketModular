#include "TeaPacket/Graphics/Graphics.hpp"

#include "GraphicsHeap/GraphicsHeap.hpp"

#include <exception>
#include <stdexcept>

#include <gx2/clear.h>
#include <gx2/context.h>
#include <gx2/display.h>
#include <gx2/event.h>
#include <gx2/mem.h>
#include <gx2/registers.h>
#include <gx2/shaders.h>
#include <gx2/state.h>
#include <gx2/surface.h>
#include <gx2/swap.h>
#include <gx2/temp.h>
#include <gx2r/mem.h>

#include <whb/gfx.h>
#include <coreinit/memdefaultheap.h>


#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Logging/Logging.hpp"

#define WHB_GFX_COMMAND_BUFFER_POOL_SIZE (0x400000)

using namespace TeaPacket;
using namespace TeaPacket::GX2;

static void* commandBufferPool = nullptr;

// I almost guarantee you there is a memory leak, or otherwise bad memory issue somewhere in this code.
// Unfortunately, I don't know where! HAHA!

void Graphics::Initialize()
{
    commandBufferPool = MEMAllocFromDefaultHeapEx(
        WHB_GFX_COMMAND_BUFFER_POOL_SIZE,
        GX2_COMMAND_BUFFER_ALIGNMENT);

    if (!commandBufferPool)
    {
        throw std::runtime_error("Invalid Texture Filter");
    }

    uint32_t initAttribs[] = {
        GX2_INIT_CMD_BUF_BASE, reinterpret_cast<uintptr_t>(commandBufferPool),
        GX2_INIT_CMD_BUF_POOL_SIZE, WHB_GFX_COMMAND_BUFFER_POOL_SIZE,
        GX2_INIT_ARGC, 0,
        GX2_INIT_ARGV, 0,
        GX2_INIT_END
    };
    GX2Init(initAttribs);
    InitializeMemory();
    
}

void Graphics::DeInitialize()
{
    Display::DeInitialize();
    DeInitializeMemory();
    GX2Shutdown();
    MEMFreeToDefaultHeap(commandBufferPool);
}

void Graphics::DrawMesh()
{
    
}
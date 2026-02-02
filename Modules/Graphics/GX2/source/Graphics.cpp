#include "TeaPacket/Graphics/Graphics.hpp"

#include "GraphicsHeap/GraphicsHeap.hpp"

#include <stdexcept>

#include <gx2/clear.h>
#include <gx2/display.h>
#include <gx2/shaders.h>
#include <gx2/state.h>
#include <coreinit/memdefaultheap.h>
#include <gx2/context.h>
#include <gx2/draw.h>
#include <gx2/registers.h>

#include "TeaPacket/Graphics/PlatformMesh.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Logging/Logging.hpp"

#include "CafeGLSL/CafeGLSLCompiler.hpp"
#include "GraphicsHeap/MEM2Resource.hpp"
#include "TeaPacket/Graphics/Mesh/Mesh.hpp"

#define WHB_GFX_COMMAND_BUFFER_POOL_SIZE (0x400000)

using namespace TeaPacket;
using namespace TeaPacket::GX2;

static void* commandBufferPool = nullptr;

// I almost guarantee you there is a memory leak, or otherwise bad memory issue somewhere in this code.
// Unfortunately, I don't know where! HAHA!

static MEM2Resource<GX2ContextState> contextState(GX2_CONTEXT_STATE_ALIGNMENT);

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
    GLSL_Init();


    GX2SetPolygonControl(
        GX2_FRONT_FACE_CW,
        FALSE,
        TRUE,
        TRUE,
        GX2_POLYGON_MODE_TRIANGLE,
        GX2_POLYGON_MODE_TRIANGLE,
        FALSE,
        FALSE,
        FALSE
        );

    assert(contextState);
    GX2SetupContextStateEx(contextState.get(), TRUE);
    GX2SetContextState(contextState.get());

    GX2SetAlphaTest(GX2_TRUE, GX2_COMPARE_FUNC_GREATER, 0.0f);
    GX2SetColorControl(GX2_LOGIC_OP_COPY, 0xFF, TRUE, TRUE);
    GX2SetBlendControl(GX2_RENDER_TARGET_0,
                /* RGB = [srcRGB * srcA] + [dstRGB * (1-srcA)] */
                GX2_BLEND_MODE_SRC_ALPHA, GX2_BLEND_MODE_INV_SRC_ALPHA,
                GX2_BLEND_COMBINE_MODE_ADD,
                FALSE,
                /* A = [srcA * 1] + [dstA * (1-srcA)] */
                GX2_BLEND_MODE_INV_SRC_ALPHA, GX2_BLEND_MODE_ONE,
                GX2_BLEND_COMBINE_MODE_ADD);
    
}

void Graphics::DeInitialize()
{
    GLSL_Shutdown();
    Display::DeInitialize();
    DeInitializeMemory();
    GX2Shutdown();
    MEMFreeToDefaultHeap(commandBufferPool);
}

void Graphics::DrawMesh()
{
    const Mesh* meshToDraw = Mesh::activeMesh;
    assert(meshToDraw != nullptr);
    
    if (meshToDraw->hasIndex)
    {
        GX2DrawIndexedEx(
            GX2_PRIMITIVE_MODE_TRIANGLES,
            meshToDraw->platformMesh->indexCount,
            GX2_INDEX_TYPE_U32,
            meshToDraw->platformMesh->indexBuffer.data(),
            0,
            1);
    }
    
}

void Graphics::SetDepthEnabled(bool depthEnabled)
{
    GX2SetDepthOnlyControl(depthEnabled, depthEnabled, GX2_COMPARE_FUNC_LESS);
}

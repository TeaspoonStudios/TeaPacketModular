#include "GraphicsHeap/GraphicsHeap.hpp"

#include <stdexcept>

#include <proc_ui/procui.h>

#include <coreinit/memexpheap.h>
#include <coreinit/memfrmheap.h>
#include <coreinit/memheap.h>
#include <gx2r/mem.h>

using namespace TeaPacket::GX2;
using namespace TeaPacket::GX2::_impl;

static bool hasForeground = false;

#define GFX_FRAME_HEAP_TAG (0x123DECAF)

static uint32_t InitializeBuckets([[maybe_unused]] void* context)
{
    hasForeground = true;
    // Mostly copied from GfxHeapInitMEM1 in whb/gfx_heap.c
    
    MEMHeapHandle heap = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM1);

    if (!MEMRecordStateForFrmHeap(heap, GFX_FRAME_HEAP_TAG))
    {
        throw std::runtime_error("MEMRecordStateForFrmHeap failed");
    }

    uint32_t size = MEMGetAllocatableSizeForFrmHeapEx(heap, 4);
    if (!size)
    {
        throw std::runtime_error("MEMGetAllocatableSizeForFrmHeapEx failed");
    }

    void* base = MEMAllocFromFrmHeapEx(heap, size, 4);
    if (!base)
    {
        throw std::runtime_error("MEMGetAllocatableSizeForFrmHeapEx failed");
    }

    GfxHeapMEM1 = MEMCreateExpHeapEx(base, size, 0);
    if (!GfxHeapMEM1)
    {
        throw std::runtime_error("MEMCreateExpHeapEx failed");
    }


    // Mostly copied frm GfxHeapInitForeground

    heap = MEMGetBaseHeapHandle(MEM_BASE_HEAP_FG);

    size = MEMGetAllocatableSizeForFrmHeapEx(heap, 4);
    if (!size) {
        throw std::runtime_error("MEMGetAllocatableSizeForFrmHeapEx failed");
    }

    base = MEMAllocFromFrmHeapEx(heap, size, 4);
    if (!base) {
        throw std::runtime_error("MEMAllocFromFrmHeapEx failed");
    }

    GfxHeapForeground = MEMCreateExpHeapEx(base, size, 0);
    if (!GfxHeapForeground) {
        throw std::runtime_error("MEMCreateExpHeapEx failed");
    }
    return 0;
}

static uint32_t FreeBuckets([[maybe_unused]] void* context)
{
    if (!hasForeground)
    {
        return 0;
    }
    hasForeground = false;
    MEMHeapHandle heap = MEMGetBaseHeapHandle(MEM_BASE_HEAP_MEM1);
    if (GfxHeapMEM1)
    {
        MEMDestroyExpHeap(GfxHeapMEM1);
        GfxHeapMEM1 = nullptr;
    }
    MEMFreeByStateToFrmHeap(heap, GFX_FRAME_HEAP_TAG);

    heap = MEMGetBaseHeapHandle(MEM_BASE_HEAP_FG);
    if (GfxHeapForeground)
    {
        MEMDestroyExpHeap(GfxHeapForeground);
        GfxHeapForeground = nullptr;
    }
    MEMFreeToFrmHeap(heap, MEM_FRM_HEAP_FREE_ALL);
    return 0;
}

void TeaPacket::GX2::InitializeMemory()
{
    InitializeBuckets(nullptr);
    ProcUIRegisterCallback(
        PROCUI_CALLBACK_ACQUIRE,
        InitializeBuckets, nullptr,
        GX2_INITMEM1_PRIORITY);
    ProcUIRegisterCallback(
        PROCUI_CALLBACK_RELEASE,
        FreeBuckets, nullptr,
        GX2_INITMEM1_PRIORITY);
    ProcUIRegisterCallback(
        PROCUI_CALLBACK_EXIT,
        FreeBuckets, nullptr,
        GX2_INITMEM1_PRIORITY);

    SetupGX2R();
}

void TeaPacket::GX2::DeInitializeMemory()
{
    FreeBuckets(nullptr);
    ProcUIClearCallbacks();
    DeInitGX2R();
}

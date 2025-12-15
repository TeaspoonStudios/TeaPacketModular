#include "GraphicsHeap/GraphicsHeap.hpp"

#include <coreinit/memexpheap.h>
#include <coreinit/memdefaultheap.h>
#include <gx2r/mem.h>
#include <gx2r/resource.h>

#include "../../../../Logging/Interface/include/TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket::GX2;
using namespace TeaPacket::GX2::_impl;

static constexpr GX2RResourceFlags MEM1ResourceTypes =
    GX2R_RESOURCE_BIND_COLOR_BUFFER |
    GX2R_RESOURCE_BIND_DEPTH_BUFFER |
    GX2R_RESOURCE_BIND_SCAN_BUFFER  |
    GX2R_RESOURCE_USAGE_FORCE_MEM1;
static bool ShouldGX2RResourceBeMEM1(const GX2RResourceFlags flags)
{
    return flags & MEM1ResourceTypes && !(flags & GX2R_RESOURCE_USAGE_FORCE_MEM2);
}

static void* GX2RAlloc(const GX2RResourceFlags flags, const uint32_t size, const uint32_t alignment)
{
    //TeaPacket::LogString("GX2RAlloc");
    const int adjustedAlignment = CorrectAlignment(alignment);
    
    if (ShouldGX2RResourceBeMEM1(flags))
    {
        return MEMAllocFromExpHeapEx(GfxHeapMEM1, size, adjustedAlignment);
    }
    // MEM2 Allocation
    return MEMAllocFromDefaultHeapEx(size, adjustedAlignment);
}

static void GX2RFree(const GX2RResourceFlags flags, void* block)
{
    //TeaPacket::LogString("GX2RFree");
    if (ShouldGX2RResourceBeMEM1(flags))
    {
        MEMFreeToExpHeap(GfxHeapMEM1, block);
        return;
    }
    MEMFreeToDefaultHeap(block);
}

void _impl::SetupGX2R()
{
    GX2RSetAllocator(&GX2RAlloc, &GX2RFree);
}

void _impl::DeInitGX2R()
{
    GX2RSetAllocator(nullptr, nullptr);
}
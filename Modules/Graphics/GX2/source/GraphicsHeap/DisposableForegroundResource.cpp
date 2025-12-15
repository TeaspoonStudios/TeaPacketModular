#include "GraphicsHeap/DisposableForegroundResource.hpp"

#include <coreinit/memexpheap.h>
#include <proc_ui/procui.h>

#include "TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket::GX2;
using namespace TeaPacket::GX2::_impl;

uint32_t DisposableForegroundMemResource::Allocate(void* resource)
{
    const auto memResource = static_cast<DisposableForegroundMemResource*>(resource);

    memResource->data = MEMAllocFromExpHeapEx(
        GetMemHeapHandleFromType(memResource->bucket),
        memResource->size,
        memResource->alignment
        );
    if (!memResource->data)
    {
        LogString("HEAP ALLOCATION FAILED!");
        return -1;
    }
    if (memResource->setupFunction != nullptr)
    {
        memResource->setupFunction(memResource);
    }
    return 0;
}

uint32_t DisposableForegroundMemResource::Deallocate(void* resource)
{
    
    const auto memResource = static_cast<DisposableForegroundMemResource*>(resource);
    if (memResource->data == nullptr)
    {
        return 0;
    }
    MEMFreeToExpHeap(
        GetMemHeapHandleFromType(memResource->bucket),
        memResource->data
        );
    
    memResource->data = nullptr;
    return 0;
}


bool DisposableForegroundMemResource::Initialize(
    const ForegroundBucket bucket, const uint32_t size, const int alignment,
    const decltype(setupFunction)& setupFunction)
{
    this->bucket = bucket;
    this->size = size;
    this->alignment = CorrectAlignment(alignment);
    this->setupFunction = setupFunction;
    
    ProcUIRegisterCallback(PROCUI_CALLBACK_ACQUIRE, Allocate, this, GX2_DISPRESOURCE_PRIORITY);
    ProcUIRegisterCallback(PROCUI_CALLBACK_RELEASE, Deallocate, this, GX2_DISPRESOURCE_PRIORITY);
    ProcUIRegisterCallback(PROCUI_CALLBACK_EXIT, Deallocate, this, GX2_DISPRESOURCE_PRIORITY);
    return Allocate(this) == 0;
}

DisposableForegroundMemResource::DisposableForegroundMemResource(
    DisposableForegroundMemResource&& other) noexcept :
bucket(ForegroundBucket::Foreground),
size(0),
alignment(0),
setupFunction(nullptr),
data(nullptr)
{
    std::swap(bucket, other.bucket);
    std::swap(size, other.size);
    std::swap(alignment, other.alignment);
    std::swap(setupFunction, other.setupFunction);
    std::swap(data, other.data);
}

DisposableForegroundMemResource::~DisposableForegroundMemResource()
{
    Deallocate(this);
}
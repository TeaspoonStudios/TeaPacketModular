#pragma once

#include <cstdint>
#include <functional>
#include <coreinit/memheap.h>

namespace TeaPacket::GX2
{
    void InitializeMemory();
    void DeInitializeMemory();

    enum class ForegroundBucket : bool
    {
        Foreground = false,
        MEM1 = true
    };

    namespace _impl
    {
        inline MEMHeapHandle GfxHeapMEM1 = nullptr;
        inline MEMHeapHandle GfxHeapForeground = nullptr;

        inline constexpr uint32_t GX2_INITMEM1_PRIORITY = 10;
        static constexpr uint32_t GX2_DISPRESOURCE_PRIORITY = 20;

        void SetupGX2R();
        void DeInitGX2R();

        
        inline int CorrectAlignment(const int alignment) { return std::max(alignment, 4); }
        inline int CorrectAlignment(const uint32_t alignment) { return CorrectAlignment(static_cast<int>(alignment)); }

        inline MEMHeapHandle& GetMemHeapHandleFromType(const ForegroundBucket bucketType)
        { return bucketType == ForegroundBucket::Foreground ? GfxHeapForeground : GfxHeapMEM1; }
    }
    
}

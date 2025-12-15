#pragma once

#include <cstdint>
#include <functional>
#include <coreinit/memdefaultheap.h>

#include "../../../../Logging/Interface/include/TeaPacket/Logging/Logging.hpp"

namespace TeaPacket::GX2
{
    template<typename T>
    class MEM2Resource
    {
    private:
        T* data = nullptr;
        int alignment = 0;
        bool Allocate()
        {
            data = static_cast<T*>(MEMAllocFromDefaultHeapEx(sizeof(T), alignment));
            return data == nullptr;
        }
    public:
        
        
        // No Copy
        MEM2Resource& operator=(const MEM2Resource&) = delete;
        MEM2Resource(const MEM2Resource&) = delete;
        
        // Constructors
        MEM2Resource() = default;
        explicit MEM2Resource(const int alignment):alignment(alignment)
        {
            Allocate();
        }

        ~MEM2Resource()
        {
            if (data == nullptr){ return; }
            MEMFreeToDefaultHeap(data);
        }

        MEM2Resource(MEM2Resource&& other) noexcept
        {
            std::swap(alignment, other.alignment);
            std::swap(data, other.data);
        }

        T& operator*()
        {
            assert(data != nullptr);
            return *data;
        }

        const T& operator*() const
        {
            assert(data != nullptr);
            return *data;
        }

        T* get() const
        {
            return data;
        }

        explicit operator bool() const
        {
            return data != nullptr;
        }
        
    };
}

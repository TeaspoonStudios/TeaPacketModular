#pragma once

#include <cstdint>
#include <functional>
#include <coreinit/memdefaultheap.h>

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
            DeAllocate();
            data = static_cast<T*>(MEMAllocFromDefaultHeapEx(sizeof(T), alignment));
            return data == nullptr;
        }

        void DeAllocate()
        {
            if (data == nullptr){ return; }
            MEMFreeToDefaultHeap(data);
        }
    public:
        
        
        // No Copy
        MEM2Resource& operator=(const MEM2Resource& other)
        {
            Allocate();
            memcpy(data, other.data, sizeof(T));
        };
        MEM2Resource(const MEM2Resource& other)
        {
            Allocate();
            memcpy(data, other.data, sizeof(T));
        };
        
        // Constructors
        MEM2Resource() = default;
        explicit MEM2Resource(const int alignment):alignment(alignment)
        {
            Allocate();
        }

        ~MEM2Resource()
        {
            DeAllocate();
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

        [[nodiscard]] T* get() const
        {
            return data;
        }

        explicit operator bool() const
        {
            return data != nullptr;
        }
        
    };

    template<>
    class MEM2Resource<void>
    {
    private:
        void* data = nullptr;
        int alignment = 0;
        size_t size = 0;
        bool Allocate()
        {
            DeAllocate();
            data = MEMAllocFromDefaultHeapEx(size, alignment);
            return data == nullptr;
        }
        void DeAllocate()
        {
            if (data == nullptr){ return; }
            MEMFreeToDefaultHeap(data);
        }
    public:
        
        
        // No Copy
        MEM2Resource& operator=(const MEM2Resource& other)
        {
            Allocate();
            memcpy(data, other.data, size);
        };
        MEM2Resource(const MEM2Resource& other)
        {
            Allocate();
            memcpy(data, other.data, size);
        };
        
        // Constructors
        MEM2Resource() = default;
        explicit MEM2Resource(const int alignment, const size_t size):alignment(alignment), size(size)
        {
            Allocate();
        }

        ~MEM2Resource()
        {
            DeAllocate();
        }

        MEM2Resource(MEM2Resource&& other) noexcept
        {
            std::swap(alignment, other.alignment);
            std::swap(data, other.data);
            std::swap(size, other.size);
        }

        [[nodiscard]] void* get() const
        {
            return data;
        }

        explicit operator bool() const
        {
            return data != nullptr;
        }
        
    };
}

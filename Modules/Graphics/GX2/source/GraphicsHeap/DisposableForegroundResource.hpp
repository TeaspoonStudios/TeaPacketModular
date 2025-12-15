#pragma once

#include <cstdint>
#include <functional>

#include "GraphicsHeap.hpp"


namespace TeaPacket::GX2
{
    enum class ForegroundBucket : bool;
    class DisposableForegroundMemResource
    {
    private:
        ForegroundBucket bucket;
        uint32_t size;
        int alignment;
        std::function<void(DisposableForegroundMemResource*)> setupFunction;

        static uint32_t Allocate(void* resource);
        static uint32_t Deallocate(void* resource);
    
    public:
        void* data;

        bool Initialize(ForegroundBucket bucket, uint32_t size, int alignment,
            const decltype(setupFunction)& setupFunction = nullptr);

        
        DisposableForegroundMemResource& operator=(const DisposableForegroundMemResource&) = delete;
        //DisposableForegroundMemResource& operator=(DisposableForegroundMemResource&&) = delete;
        DisposableForegroundMemResource(const DisposableForegroundMemResource&) = delete;
        DisposableForegroundMemResource(DisposableForegroundMemResource&& other) noexcept;

        // Default Empty Constructor
        DisposableForegroundMemResource() :
            bucket(static_cast<ForegroundBucket>(false)),
            size(0),
            alignment(0),
            data(nullptr)
        {}
        ~DisposableForegroundMemResource();

        // Getters
        [[nodiscard]] decltype(size) GetSize() const { return size; }
        [[nodiscard]] decltype(alignment) GetAlignment() const { return alignment; }
        [[nodiscard]] decltype(bucket) GetBucket() const { return bucket; }
    };
}

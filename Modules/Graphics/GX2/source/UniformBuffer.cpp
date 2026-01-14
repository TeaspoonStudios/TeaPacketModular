#include "TeaPacket/Graphics/UniformBuffer.hpp"

#include <cstring>
#include <gx2/mem.h>
#include <gx2/shaders.h>

#include "GraphicsHeap/MEM2Resource.hpp"
#include "TeaPacket/Graphics/UniformBufferParameters.hpp"
#include "TeaPacket/Graphics/PlatformUniformBuffer.hpp"

using namespace TeaPacket::Graphics;

UniformBuffer::UniformBuffer(const UniformBufferParameters& parameters):
    size(parameters.size),
    platformBuffer(std::make_unique<PlatformUniformBuffer>(PlatformUniformBuffer{
        .data = GX2::MEM2Resource<void>(0x100, size)
    }))
{
    if (parameters.data != nullptr)
    {
        memcpy(platformBuffer->data.get(), parameters.data, size);
    } else
    {
        memset(platformBuffer->data.get(), 0, size);
    }
}

void UniformBuffer::SetActive(const size_t index)
{
    GX2SetVertexUniformBlock(index, size, platformBuffer->data.get());
    GX2SetPixelUniformBlock (index, size, platformBuffer->data.get());
}

void UniformBuffer::SendData(const void* data)
{
    memcpy(platformBuffer->data.get(), data, size);
    GX2Invalidate(GX2_INVALIDATE_MODE_CPU | GX2_INVALIDATE_MODE_UNIFORM_BLOCK, platformBuffer->data.get(), size);
}

UniformBuffer::~UniformBuffer() = default;

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept = default;
UniformBuffer::UniformBuffer(const UniformBuffer& other):
size(other.size)
{
    platformBuffer = std::make_unique<PlatformUniformBuffer>(*other.platformBuffer);
}

constexpr bool UniformBuffer::ShouldUBBeEndianSwapped = true;
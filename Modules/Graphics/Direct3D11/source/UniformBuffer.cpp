#include "TeaPacket/Graphics/UniformBuffer.hpp"

#include <d3d11.h>

#include <memory>

#include "TeaPacket/Graphics/UniformBufferParameters.hpp"
#include "TeaPacket/Graphics/PlatformUniformBuffer.hpp"
#include "TeaPacket/Graphics/WindowsGraphics.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

using namespace TeaPacket::Graphics;

UniformBuffer::UniformBuffer(const UniformBufferParameters& parameters):
    size(parameters.size),
    platformBuffer(std::make_unique<PlatformUniformBuffer>())
{
    const auto bufferDesc = D3D11_BUFFER_DESC{
        .ByteWidth = static_cast<UINT>(parameters.size),
        .Usage = D3D11_USAGE_DYNAMIC,
        .BindFlags = D3D11_BIND_CONSTANT_BUFFER,
        .CPUAccessFlags = D3D11_CPU_ACCESS_WRITE,
        .MiscFlags = 0,
        .StructureByteStride = 0,
    };

    const auto subResourceData = D3D11_SUBRESOURCE_DATA{
        .pSysMem = parameters.data,
        .SysMemPitch = 0,
        .SysMemSlicePitch = 0,
    };
    

    CheckErrorWinCom(
        device->CreateBuffer(
            &bufferDesc,
            parameters.data == nullptr ? nullptr : &subResourceData,
            platformBuffer->cbuffer.GetAddressOf())
        );
    
}

void UniformBuffer::SetActive(const size_t index)
{
    deviceContext->VSSetConstantBuffers(static_cast<UINT>(index), 1, platformBuffer->cbuffer.GetAddressOf());
    deviceContext->PSSetConstantBuffers(static_cast<UINT>(index), 1, platformBuffer->cbuffer.GetAddressOf());
}

void UniformBuffer::SendData(const void* data)
{
    ID3D11Buffer* bufferPtr = platformBuffer->cbuffer.Get();
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CheckErrorWinCom( deviceContext->Map(
       bufferPtr,
       0,
       D3D11_MAP_WRITE_DISCARD,
       0,
       &mappedResource
    ));
    memcpy(mappedResource.pData, data, size);
    deviceContext->Unmap(bufferPtr, 0);
}

constexpr bool UniformBuffer::ShouldUBBeEndianSwapped = false;

UniformBuffer::UniformBuffer(UniformBuffer&& other) noexcept = default;
UniformBuffer::UniformBuffer(const UniformBuffer& other):
size(other.size)
{
    platformBuffer = std::make_unique<PlatformUniformBuffer>(*other.platformBuffer);
}

UniformBuffer::~UniformBuffer() = default;

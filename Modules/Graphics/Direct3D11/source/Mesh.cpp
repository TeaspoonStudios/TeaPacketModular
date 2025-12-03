#include "TeaPacket/Graphics/Mesh/Mesh.hpp"
#include "TeaPacket/Graphics/PlatformMesh.hpp"

#include <d3d11.h>
#include <cassert>

#include "TeaPacket/Graphics/WindowsGraphics.hpp"
#include "TeaPacket/Graphics/Mesh/MeshParameters.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"
#include "TeaPacket/Types/Enums/PrimitiveTypes.hpp"

using namespace TeaPacket::Graphics;

Mesh::Mesh(const MeshParameters& parameters):
    platformMesh(std::make_unique<PlatformMesh>()),
    vertexDataInfo(parameters.vertexInfo),
    hasIndex(parameters.flags.useIndices)
{
    size_t vertexSize = 0;
    for (const VertexDataInfo& vertexData : parameters.vertexInfo)
    {
        vertexSize += SizeOfPrimitiveType(vertexData.type) * vertexData.size;
    }
    platformMesh->vertexSize = static_cast<UINT>(vertexSize);

    D3D11_BUFFER_DESC vertexBufferDesc;
    D3D11_SUBRESOURCE_DATA d3dVertexData;

    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = static_cast<unsigned int>(parameters.vertexData.size);
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
    vertexBufferDesc.StructureByteStride = 0;

    d3dVertexData.pSysMem = parameters.vertexData.data;
    d3dVertexData.SysMemPitch = 0;
    d3dVertexData.SysMemSlicePitch = 0;

    CheckErrorWinCom(
        device->CreateBuffer(&vertexBufferDesc, &d3dVertexData, platformMesh->vertexBuffer.GetAddressOf())
    );

    if (parameters.flags.useIndices)
    {
        assert(parameters.indices.has_value());

        D3D11_BUFFER_DESC indexBufferDesc;
        indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
        indexBufferDesc.ByteWidth = static_cast<UINT>(parameters.indices->SizeOf());
        indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        indexBufferDesc.CPUAccessFlags = 0;
        indexBufferDesc.MiscFlags = 0;
        indexBufferDesc.StructureByteStride = 0;

        D3D11_SUBRESOURCE_DATA d3dIndexData;
        d3dIndexData.pSysMem = parameters.indices.value().data;
        d3dIndexData.SysMemPitch = 0;
        d3dIndexData.SysMemSlicePitch = 0;

        CheckErrorWinCom(
            device->CreateBuffer(&indexBufferDesc, &d3dIndexData, platformMesh->indexBuffer.GetAddressOf())
        );

        platformMesh->indexCount = static_cast<UINT>(parameters.indices->size);
    }
}

void Mesh::SetActive()
{
    activeMesh = this;

    constexpr unsigned int offset = 0;
    deviceContext->IASetVertexBuffers(0, 1, platformMesh->vertexBuffer.GetAddressOf(), &platformMesh->vertexSize, &offset);
    if (hasIndex)
    {
        deviceContext->IASetIndexBuffer(platformMesh->indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
        deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    }
}

Mesh::~Mesh() = default;

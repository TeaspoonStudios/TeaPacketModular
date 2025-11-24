#include "TeaPacket/Graphics/Shader/Shader.hpp"

#include <vector>

#include <d3dcompiler.h>
#include <wrl/client.h>

#include "TeaPacket/Graphics/PlatformShader.hpp"
#include "TeaPacket/Graphics/WindowsGraphics.hpp"

#include "TeaPacket/Graphics/Shader/ShaderParameters.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

#include "TeaPacket/Logging/Logging.hpp"
#include "TeaPacket/MacroUtils/StructUtils.hpp"

using namespace TeaPacket::Graphics;


static constexpr DXGI_FORMAT GetDXGIFormatFromVertexAttribute(ShaderVariableType shaderVarType)
{
    using enum ShaderVariableBaseType;
    switch (shaderVarType.amount)
    {
    case 1:
        switch (shaderVarType.baseType)
        {
    case Float:     return DXGI_FORMAT_R32_FLOAT;
    case Int:       return DXGI_FORMAT_R32_SINT;
    case UInt:      return DXGI_FORMAT_R32_UINT;
    case Texture:   return DXGI_FORMAT_UNKNOWN;
        }
    case 2:
        switch (shaderVarType.baseType)
        {
    case Float: return DXGI_FORMAT_R32G32_FLOAT;
    case Int: return DXGI_FORMAT_R32G32_SINT;
    case UInt: return DXGI_FORMAT_R32G32_UINT;
    case Texture: return DXGI_FORMAT_UNKNOWN;
        }
    case 3:
        switch (shaderVarType.baseType)
        {
    case Float: return DXGI_FORMAT_R32G32B32_FLOAT;
    case Int: return DXGI_FORMAT_R32G32B32_SINT;
    case UInt: return DXGI_FORMAT_R32G32B32_UINT;
    case Texture: return DXGI_FORMAT_UNKNOWN;
        }
    case 4:
        switch (shaderVarType.baseType)
        {
    case Float: return DXGI_FORMAT_R32G32B32A32_FLOAT;
    case Int: return DXGI_FORMAT_R32G32B32A32_SINT;
    case UInt: return DXGI_FORMAT_R32G32B32A32_UINT;
    case Texture: return DXGI_FORMAT_UNKNOWN;
        }
    default: throw std::exception();
    }
}

Shader::Shader(const ShaderParameters& parameters):
platformShader(std::make_unique<PlatformShader>()),
uniformBufferSizes(parameters.uniformBufferSizes)
{
    Microsoft::WRL::ComPtr<ID3D10Blob> errorMessage, vertexShaderBuffer;
    // Compile Vertex Shader
    HRESULT result = D3DCompile(
        parameters.vertexShaderCode.c_str(), parameters.vertexShaderCode.length(),
        NULL, NULL, NULL,
        "main", "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS, 0,
        vertexShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()
        );
    // Check Vertex Shader Compilation for errors
    if (FAILED(result))
    {
        if (errorMessage.Get())
        {
            const char* compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
            const size_t size = errorMessage->GetBufferSize();
            LogString(std::string(compileErrors, compileErrors + size));
            throw std::runtime_error("Vertex shader failed to compile. See Log for errors.");
        }
        throw std::runtime_error("Vertex shader couldn't compile. Error message not given. Uh Oh.");
    }

    Microsoft::WRL::ComPtr<ID3D10Blob> pixelShaderBuffer;
    // Compile Pixel Shader
    result =
        D3DCompile(
            parameters.fragmentShaderCode.c_str(), parameters.fragmentShaderCode.length(),
            NULL, NULL, NULL,
            "main", "ps_5_0",
            D3D10_SHADER_ENABLE_STRICTNESS, 0,
            pixelShaderBuffer.GetAddressOf(), errorMessage.GetAddressOf()
            );
    // Check Pixel Shader for errors
    if (FAILED(result))
    {
        if (errorMessage.Get())
        {
            const char* compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
            const size_t size = errorMessage->GetBufferSize();
            LogString(std::string(compileErrors, compileErrors + size));
            throw std::runtime_error("Fragment shader failed to compile. See Log for errors.");
        }
        throw std::runtime_error("Fragment shader couldn't compile. Error message not given. Uh Oh.");
    }
    
    // Create both shaders
    CheckErrorWinCom(
        device->CreateVertexShader(
            vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
            NULL, platformShader->vertexShader.GetAddressOf()
            ));

    CheckErrorWinCom(
        device->CreatePixelShader(
            pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(),
            NULL, platformShader->pixelShader.GetAddressOf()
    ));
    // Setup input variables
    std::vector<D3D11_INPUT_ELEMENT_DESC> polygonLayout;
    polygonLayout.reserve(parameters.inputAttributes.size());
    for (size_t i = 0; i < parameters.inputAttributes.size(); i++)
    {
        polygonLayout.emplace_back(D3D11_INPUT_ELEMENT_DESC{
            .SemanticName = "TEXCOORD",
            .SemanticIndex = static_cast<unsigned int>(i),
            .Format = GetDXGIFormatFromVertexAttribute(parameters.inputAttributes[i]),
            .InputSlot = 0,
            .AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT,
            .InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA,
            .InstanceDataStepRate = 0
        });
    }
    
    CheckErrorWinCom(
        device->CreateInputLayout(polygonLayout.data(), static_cast<UINT>(parameters.inputAttributes.size()),
            vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
            platformShader->inputLayout.GetAddressOf()
            ));

    // Setup CBuffers
    auto cbufferDesc = D3D11_BUFFER_DESC();
    platformShader->cbuffers.resize(parameters.uniformBufferSizes.size());
    auto initData = D3D11_SUBRESOURCE_DATA();
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;

    for (size_t i = 0; i < parameters.uniformBufferSizes.size(); i++)
    {
        cbufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        cbufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        cbufferDesc.ByteWidth = static_cast<UINT>(parameters.uniformBufferSizes[i]);
        cbufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        cbufferDesc.MiscFlags = 0;
        cbufferDesc.StructureByteStride = 0;

        const auto* zeroData = new unsigned char[parameters.uniformBufferSizes[i]]();
        initData.pSysMem = zeroData;

        CheckErrorWinCom(
            device->CreateBuffer(&cbufferDesc, &initData, platformShader->cbuffers[i].GetAddressOf())
            );
        delete[] zeroData;
        initData.pSysMem = nullptr;
    }

    
}
void Shader::SetActive()
{
    deviceContext->IASetInputLayout(platformShader->inputLayout.Get());
    deviceContext->VSSetShader(platformShader->vertexShader.Get(), NULL, 0);
    deviceContext->PSSetShader(platformShader->pixelShader.Get(), NULL, 0);

    // Add Uniform support later
}

void Shader::SendUniformBuffer(const unsigned char* data, const size_t bufferIndex)
{
    ID3D11Buffer* bufferPtr = platformShader->cbuffers[bufferIndex].Get();
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CheckErrorWinCom(deviceContext->Map(
        bufferPtr,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource));

    memcpy(mappedResource.pData, data, uniformBufferSizes[bufferIndex]);
    deviceContext->Unmap(bufferPtr, 0);

    deviceContext->VSSetConstantBuffers(static_cast<UINT>(bufferIndex), 1, &bufferPtr);
    deviceContext->PSSetConstantBuffers(static_cast<UINT>(bufferIndex), 1, &bufferPtr);
}



Shader::~Shader() = default;

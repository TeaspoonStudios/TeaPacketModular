#include "TeaPacket/Graphics/Texture/Texture.hpp"

#include <d3d11.h>

#include "TeaPacket/Graphics/Texture/TextureParameters.hpp"
#include "TeaPacket/Graphics/PlatformTexture.hpp"
#include "TeaPacket/Graphics/Texture/TextureFormat.hpp"
#include "TeaPacket/Graphics/WindowsGraphics.hpp"
#include "TeaPacket/MacroUtils/StructUtils.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"
#include "TeaPacket/Graphics/Texture/TextureData.hpp"

#include "TeaPacket/Graphics/D3D11/D3D11TextureFormat.gen"
#include "TeaPacket/Graphics/D3D11/D3D11TextureFilter.gen"
#include "TeaPacket/Graphics/D3D11/D3D11TextureWrap.gen"

using namespace TeaPacket::Graphics;

static constexpr D3D11_USAGE GetD3DUsage(const TextureUseFlags flags)
{
    switch (flags.writeMode)
    {
    case TextureAvailableMode::None:
        return D3D11_USAGE_IMMUTABLE;
    case TextureAvailableMode::CPU:
        return D3D11_USAGE_DYNAMIC;
    case TextureAvailableMode::GPU:
        return D3D11_USAGE_DEFAULT;
    default:
        throw std::runtime_error("Invalid TextureWriteMode");
    }
}

static constexpr D3D11_BIND_FLAG GetD3DBindFlags(const TextureUseFlags flags)
{
    UINT flag = 0;
    if (flags.shaderResource) { flag |= D3D11_BIND_SHADER_RESOURCE; }
    if (flags.renderTargetColor) { flag |= D3D11_BIND_RENDER_TARGET; }
    if (flags.renderTargetDepth) { flag |= D3D11_BIND_DEPTH_STENCIL; }
    return static_cast<D3D11_BIND_FLAG>(flag);
}

static constexpr D3D11_CPU_ACCESS_FLAG GetD3DCpuAccessFlags(const TextureUseFlags flags)
{
    return static_cast<D3D11_CPU_ACCESS_FLAG>(
        flags.writeMode == TextureAvailableMode::CPU ? D3D11_CPU_ACCESS_WRITE : 0
        );
}

Texture::Texture(const TextureParameters& parameters):
platformTexture(std::make_unique<PlatformTexture>()),
width(parameters.width),
height(parameters.height),
format(parameters.format)
{
    D3D11_TEXTURE2D_DESC textureDesc;
    textureDesc.Width = width;
    textureDesc.Height = height;
    textureDesc.MipLevels = 1;
    textureDesc.ArraySize = 1;
    textureDesc.Format = TextureFormatToD3D(format);
    textureDesc.SampleDesc.Count = 1;
    textureDesc.SampleDesc.Quality = 0;
    textureDesc.Usage = GetD3DUsage(parameters.useFlags);
    textureDesc.BindFlags = GetD3DBindFlags(parameters.useFlags);
    textureDesc.CPUAccessFlags = GetD3DCpuAccessFlags(parameters.useFlags);
    textureDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA texData = {
        .pSysMem = parameters.data,
        .SysMemPitch = static_cast<UINT>(width) * GetTextureFormatBytesPerPixel(format),
        .SysMemSlicePitch = 0
    };

    CheckErrorWinCom(
        device->CreateTexture2D(&textureDesc,
            parameters.data == nullptr ? nullptr : &texData, platformTexture->texture2D.GetAddressOf())
    );

    if (parameters.data == nullptr)
    {
        assert(parameters.useFlags.writeMode != TextureAvailableMode::None);
    }

    if (parameters.useFlags.shaderResource)
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
        srvDesc.Format = textureDesc.Format;
        srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srvDesc.Texture2D.MostDetailedMip = 0;
        srvDesc.Texture2D.MipLevels = UINT_MAX;
        CheckErrorWinCom(
            device->CreateShaderResourceView(
                platformTexture->texture2D.Get(), &srvDesc, platformTexture->shaderResourceView.GetAddressOf()
                )
        );
        deviceContext->GenerateMips(platformTexture->shaderResourceView.Get());

        D3D11_SAMPLER_DESC samplerDesc;
        samplerDesc.Filter = TextureFilterModeToD3D(parameters.filterMode);
        D3D11_TEXTURE_ADDRESS_MODE d3dWrapType = TextureWrapModeToD3D(parameters.wrapMode);
        samplerDesc.AddressU = d3dWrapType;
        samplerDesc.AddressV = d3dWrapType;
        samplerDesc.AddressW = d3dWrapType;
        samplerDesc.MipLODBias = 0;
        samplerDesc.MaxAnisotropy = 1;
        samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
        ZeroStruct(samplerDesc.BorderColor);
        samplerDesc.MinLOD = 0;
        samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

        CheckErrorWinCom(
            device->CreateSamplerState(&samplerDesc, platformTexture->samplerState.GetAddressOf())
        );
    }
}

Texture::~Texture() = default;

TextureData Texture::GetData() const
{
    D3D11_TEXTURE2D_DESC stagingDesc;
    platformTexture->texture2D->GetDesc(&stagingDesc);
    stagingDesc.Usage = D3D11_USAGE_STAGING;
    stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    stagingDesc.BindFlags = 0;
    stagingDesc.MiscFlags = 0;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> stagingTex;
    CheckErrorWinCom(
        device->CreateTexture2D(&stagingDesc, NULL,
            stagingTex.GetAddressOf())
    );

    deviceContext->CopyResource(
        stagingTex.Get(),
        platformTexture->texture2D.Get());

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CheckErrorWinCom(
    deviceContext->Map(stagingTex.Get(),
        0, D3D11_MAP_READ, 0, &mappedResource)
    );

    const uint8_t bpp = GetTextureFormatBPP(format);
    const uint_fast16_t h = height;

    TextureData data = TextureData(bpp * mappedResource.RowPitch * height);
    data.width = static_cast<uint16_t>(stagingDesc.Width);
    data.height = static_cast<uint16_t>(stagingDesc.Height);
    data.format = D3DToTextureFormat(stagingDesc.Format);
    data.pitch = static_cast<uint16_t>(mappedResource.RowPitch);

    unsigned char* srcBeg;
    for (uint_fast16_t y = 0; y < h; y++)
    {
        srcBeg = static_cast<unsigned char*>(mappedResource.pData) + mappedResource.RowPitch * y;
        data.data.insert(data.data.end(), srcBeg, srcBeg + mappedResource.RowPitch);
    }
    deviceContext->Unmap(stagingTex.Get(),0);
    return data;
}

void Texture::SetActive(const uint8_t index)
{
    deviceContext->PSSetShaderResources(index, 1, platformTexture->shaderResourceView.GetAddressOf());
    deviceContext->PSSetSamplers(index, 1, platformTexture->samplerState.GetAddressOf());
}

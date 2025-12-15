#include "TeaPacket/Graphics/Viewport.hpp"

#include "TeaPacket/Graphics/Texture/TextureParameters.hpp"
#include "TeaPacket/Graphics/ViewportParameters.hpp"

#include <d3d11.h>
#include <iostream>
#include <cassert>

#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/PlatformDisplay.hpp"
#include "TeaPacket/Graphics/PlatformTexture.hpp"
#include "TeaPacket/Graphics/PlatformViewport.hpp"
#include "TeaPacket/Graphics/Texture/TextureFormat.hpp"
#include "TeaPacket/Graphics/WindowsGraphics.hpp"
#include "TeaPacket/MacroUtils/StructUtils.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

using namespace TeaPacket::Graphics;

Viewport::Viewport(const ViewportParameters& parameters):
colorTexture(TextureParameters{
    .data = nullptr,
    .width = parameters.width,
    .height = parameters.height,
    .format = TextureFormat::BGRA8,
    .useFlags = {
        .shaderResource = parameters.flags.shaderUsable,
        .renderTargetColor = true,
        .renderTargetDepth = false,
        .writeMode = TextureAvailableMode::GPU,
        .cpuReadable = true,
    },
}),

depthTexture(TextureParameters{
    .data = nullptr,
    .width = parameters.width,
    .height = parameters.height,
    .format = TextureFormat::D24S8,
    .useFlags = {
        .shaderResource = parameters.flags.shaderUsable,
        .renderTargetColor = false,
        .renderTargetDepth = true,
        .writeMode = TextureAvailableMode::GPU,
        .cpuReadable = true
    },
}),
platformViewport(std::make_unique<PlatformViewport>()),
ownedDisplay(parameters.ownedDisplay)
{
    D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
    D3D11_TEXTURE2D_DESC texDesc;
    colorTexture.platformTexture->texture2D->GetDesc(&texDesc);
    renderTargetViewDesc.Format = texDesc.Format;
    renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    renderTargetViewDesc.Texture2D.MipSlice = 0;

    CheckErrorWinCom(
        device->CreateRenderTargetView(
            colorTexture.platformTexture->texture2D.Get(), &renderTargetViewDesc,
            platformViewport->renderTargetView.GetAddressOf())
    );

    D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
    ZeroStruct(depthStencilDesc);
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xFF;
    depthStencilDesc.StencilWriteMask = 0xFF;

    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    CheckErrorWinCom(
        device->CreateDepthStencilState(&depthStencilDesc,
            platformViewport->depthStencilState.GetAddressOf())
    );

    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
    ZeroStruct(depthStencilViewDesc);
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    CheckErrorWinCom(
        device->CreateDepthStencilView(depthTexture.platformTexture->texture2D.Get(), &depthStencilViewDesc,
            platformViewport->depthStencilView.GetAddressOf())
    );

    platformViewport->d3dViewport = D3D11_VIEWPORT{
        .TopLeftX = 0,
        .TopLeftY = 0,
        .Width = static_cast<float>(parameters.width),
        .Height = static_cast<float>(parameters.height),
        .MinDepth = 0,
        .MaxDepth = 1,
    };
}

Viewport::~Viewport() = default;

void Viewport::BeginRender()
{
    assert(activeViewport == nullptr);
    activeViewport = this;

    deviceContext->OMSetRenderTargets(1,
        platformViewport->renderTargetView.GetAddressOf(),
        platformViewport->depthStencilView.Get());
    deviceContext->RSSetViewports(1, &platformViewport->d3dViewport);
}

void Viewport::FinishRender()
{
    activeViewport = nullptr;
}

void Viewport::ClearColor(const unsigned char r, const unsigned char g, const unsigned char b)
{
    const float colorArray[4] = {r / 255.0f, g / 255.0f, b / 255.0f, 1.0f};
    deviceContext->ClearRenderTargetView(activeViewport->platformViewport->renderTargetView.Get(),colorArray);
    deviceContext->ClearDepthStencilView(activeViewport->platformViewport->depthStencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);
}

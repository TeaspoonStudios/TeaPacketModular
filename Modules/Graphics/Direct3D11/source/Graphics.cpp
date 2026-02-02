#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/WindowsGraphics.hpp"

#include <d3d11.h>
#include <cassert>

#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/PlatformMesh.hpp"
#include "TeaPacket/Graphics/Mesh/Mesh.hpp"
#include "TeaPacket/MacroUtils/StructUtils.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

using namespace TeaPacket;

static constexpr D3D_FEATURE_LEVEL d3dFeatureLevels[] = {
    D3D_FEATURE_LEVEL_11_1,
    D3D_FEATURE_LEVEL_11_0,
    D3D_FEATURE_LEVEL_10_1,
    D3D_FEATURE_LEVEL_10_0,
    D3D_FEATURE_LEVEL_9_3,
    D3D_FEATURE_LEVEL_9_2,
    D3D_FEATURE_LEVEL_9_1,
};

static D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
static Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthStencilState = nullptr;
static Microsoft::WRL::ComPtr<ID3D11BlendState> blendState = nullptr;

void Graphics::Initialize()
{
    UINT deviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;
#if TP_DEBUG > 0
    deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif


    CheckErrorWinCom(
        D3D11CreateDevice(
            nullptr,
            D3D_DRIVER_TYPE_HARDWARE,
            0,
            deviceFlags,
            d3dFeatureLevels,
            ARRAYSIZE(d3dFeatureLevels),
            D3D11_SDK_VERSION,
            device.ReleaseAndGetAddressOf(),
            NULL,
            deviceContext.ReleaseAndGetAddressOf()
        )
    );

    constexpr auto desc = D3D11_RASTERIZER_DESC{
        .FillMode = D3D11_FILL_SOLID,
        .CullMode = D3D11_CULL_NONE,
        .FrontCounterClockwise = FALSE,
        .DepthBias = 0,
        .DepthBiasClamp = 0.0f,
        .SlopeScaledDepthBias = 0.0f,
        .DepthClipEnable = TRUE,
        .ScissorEnable = FALSE,
        .MultisampleEnable = FALSE,
        .AntialiasedLineEnable = FALSE
    };
    CheckErrorWinCom(device->CreateRasterizerState(&desc, defaultRasterizerState.GetAddressOf()));
    deviceContext->RSSetState(defaultRasterizerState.Get());

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
            depthStencilState.ReleaseAndGetAddressOf())
    );
    deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);

    D3D11_BLEND_DESC blendDesc;
    ZeroStruct(blendDesc);
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.IndependentBlendEnable = false;

    blendDesc.RenderTarget[0] = {
        .BlendEnable = true,
        .SrcBlend = D3D11_BLEND_SRC_ALPHA,
        .DestBlend = D3D11_BLEND_INV_SRC_ALPHA,
        .BlendOp = D3D11_BLEND_OP_ADD,
        .SrcBlendAlpha = D3D11_BLEND_ONE,
        .DestBlendAlpha = D3D11_BLEND_ZERO,
        .BlendOpAlpha = D3D11_BLEND_OP_ADD,
        .RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL
    };

    CheckErrorWinCom(
        device->CreateBlendState(&blendDesc, blendState.ReleaseAndGetAddressOf())
    );
    //constexpr float bf[] = { 0.f,0.f,0.f,0.f };
    deviceContext->OMSetBlendState(blendState.Get(), nullptr, 0xffffffff);
}

void Graphics::DeInitialize()
{
    device.Reset();
    deviceContext.Reset();
    Display::DeInitialize();
}

void Graphics::DrawMesh()
{
    const Mesh* meshToDraw = Mesh::activeMesh;
    assert(meshToDraw != nullptr);
    
    if (meshToDraw->hasIndex)
    {
        deviceContext->DrawIndexed(meshToDraw->platformMesh->indexCount, 0, 0);
    }
}

void Graphics::SetDepthEnabled(const bool depthEnabled)
{
    depthStencilDesc.DepthEnable = depthEnabled;
    CheckErrorWinCom(
        device->CreateDepthStencilState(&depthStencilDesc,
            depthStencilState.ReleaseAndGetAddressOf())
    );
    deviceContext->OMSetDepthStencilState(depthStencilState.Get(), 0);
}

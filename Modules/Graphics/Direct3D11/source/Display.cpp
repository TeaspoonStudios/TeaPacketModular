#include "TeaPacket/Graphics/Display.hpp"

#include "TeaPacket/Graphics/PlatformDisplay.hpp"
#include "TeaPacket/Window/Window.hpp"
#include "TeaPacket/MacroUtils/StructUtils.hpp"

#include <dxgi1_3.h>

#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/PlatformTexture.hpp"
#include "TeaPacket/Graphics/PlatformViewport.hpp"
#include "TeaPacket/Graphics/ViewportParameters.hpp"
#include "TeaPacket/Graphics/WindowsGraphics.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"
#include "TeaPacket/Window/PlatformWindow.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"


using namespace TeaPacket::Graphics;

Display::Display(const DisplayParameters& parameters):
viewport(ViewportParameters{
    .width = parameters.width,
    .height = parameters.height,
    .ownedDisplay = this
    }),
platformDisplay(std::make_unique<PlatformDisplay>())
{
    platformDisplay->window = std::make_unique<Window::Window>(Window::WindowParameters{
        .x = 0,
        .y = 0,
        .width = parameters.width,
        .height = parameters.height,
        .title = "TeaPacket",
        .display = this
    });

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroStruct(desc);
    desc.Windowed = true;
    desc.BufferCount = 2;
    desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    desc.BufferDesc.Width = GetWidth();
    desc.BufferDesc.Height = GetHeight();
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    desc.OutputWindow = platformDisplay->window->platformWindow->windowHandle;
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    Microsoft::WRL::ComPtr<IDXGIDevice3> dxgiDevice;
    CheckErrorWinCom(device.As(&dxgiDevice));

    Microsoft::WRL::ComPtr<IDXGIAdapter> adapter;
    Microsoft::WRL::ComPtr<IDXGIFactory> factory;

    CheckErrorWinCom(dxgiDevice->GetAdapter(adapter.GetAddressOf()));
    CheckErrorWinCom(adapter->GetParent(IID_PPV_ARGS(factory.GetAddressOf())));
    CheckErrorWinCom(
        factory->CreateSwapChain(device.Get(), &desc, &platformDisplay->swapchain)
    );

    CheckErrorWinCom(
        platformDisplay->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D),
                            reinterpret_cast<LPVOID*>(viewport.GetTexture()->platformTexture->texture2D.ReleaseAndGetAddressOf()))
    );

    CheckErrorWinCom(
        device->CreateRenderTargetView(viewport.GetTexture()->platformTexture->texture2D.Get(), NULL,
            viewport.platformViewport->renderTargetView.ReleaseAndGetAddressOf())
        );
}

void Display::InitializeDefaultDisplays(const std::vector<DisplayParameters>& requestedParameters)
{
    for (auto params : requestedParameters)
    {
        Displays.emplace_back(std::make_unique<Display>(params));
    }
}

void Display::PresentAll()
{
    for (const auto& display : Displays)
    {
        CheckErrorWinCom(display->platformDisplay->swapchain->Present(0,0));
    }
}

void Display::WaitForVSync()
{
    
}

Display::~Display() = default;

void Display::DeInitialize()
{
    Displays.clear();
}

#include <iostream>

#include "TeaPacket/Window/Window.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Texture/TextureData.hpp"
#include "TeaPacket/System/System.hpp"

using namespace TeaPacket::Window;

[[noreturn]] int main()
{
    TeaPacket::Graphics::Initialize();
    auto dispParams = TeaPacket::Graphics::DisplayParameters{.width = 1280, .height = 720};
    TeaPacket::Graphics::Display::InitializeDefaultDisplays({dispParams});
    TeaPacket::Graphics::Viewport* viewport = TeaPacket::Graphics::Display::GetDisplay(0)->GetViewport();

    while (true)
    {
        TeaPacket::System::ProcessSystem();
        viewport->BeginRender();
        viewport->ClearColor(255, 0, 0);
        viewport->FinishRender();
        static int number = 0;
        number++;
        if (number == 0)
        {
            auto texData = viewport->GetTexture()->GetData();
            const Color rgb = texData.GetColor4(1,1);
            std::cout << static_cast<std::string>(rgb);
        }
    }
    TeaPacket::Graphics::DeInitialize();
}
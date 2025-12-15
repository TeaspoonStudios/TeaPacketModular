#include "TeaPacket/Graphics/Graphics.hpp"

#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Graphics;

int main()
{
    TeaPacket::Initialize();

    auto dispParams = DisplayParameters{.width = 1280, .height = 720};
    Display::InitializeDefaultDisplays({dispParams});

    while (System::ShouldRun())
    {
        System::ProcessSystem();
        Display::WaitForVSync();
        Display::GetDisplay(0)->GetViewport()->BeginRender();
        Display::GetDisplay(0)->GetViewport()->FinishRender();
        Display::GetDisplay(1)->GetViewport()->BeginRender();
        Display::GetDisplay(1)->GetViewport()->FinishRender();

        Display::PresentAll();
    }
    
    TeaPacket::DeInitialize();
}
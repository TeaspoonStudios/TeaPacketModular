#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/Mesh/Mesh.hpp"
#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Core/Core.hpp"
#include "TeaPacket/Input/Input.hpp"
#include "TeaPacket/Input/InputButtons.hpp"
#include "TeaPacket/Input/InputDevice.hpp"
#include "TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Graphics;
//using namespace TeaPacket::Input;

int main()
{
    TeaPacket::Initialize();
    auto dispParams = DisplayParameters{.width = 1280, .height = 720};
    Display::InitializeDefaultDisplays({dispParams});
    
    while (System::ShouldRun())
    {
        System::ProcessSystem();
        Display::BeginRender(0);
        Viewport::ClearColor(255, 0, 0);
        
        Input::InputDevice::PollAllDevices();
        if (Input::InputDevice::AnyDeviceHasButtonPressed(Input::InputButtonType::KEY_ENTER))
        {
            Viewport::ClearColor(0, 255, 0);
        }
        Display::FinishRender(0);
        Display::PresentAll();
    }
    TeaPacket::DeInitialize();
}
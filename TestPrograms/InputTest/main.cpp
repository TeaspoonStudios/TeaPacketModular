#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/Mesh/Mesh.hpp"
#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Core/Core.hpp"
#include "TeaPacket/Input/Input.hpp"
#include "TeaPacket/Input/InputAxis.hpp"
#include "TeaPacket/Input/InputButtons.hpp"
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
        
        Input::UpdateControllers();
        Input::PollAllInputs();
        Viewport::ClearColor(0, 0, 0);
        const Input::ControllerSlot mouseSlot = Input::GetLastControllerPressed(Input::ControllerType::Mouse);
        if (mouseSlot != Input::NoControllerSlot)
        {
            Viewport::ClearColor(
                static_cast<uint8_t>(255 * Input::GetAxisValue(mouseSlot, Input::InputAxisType::POINTER_X)),
                static_cast<uint8_t>(255 * Input::GetAxisValue(mouseSlot, Input::InputAxisType::POINTER_Y)),
                0);
        }

        const Input::ControllerSlot keyboardSlot = Input::GetLastControllerPressed(Input::ControllerType::Keyboard);
        if (keyboardSlot != Input::NoControllerSlot)
        {
            if (Input::IsButtonPressed(keyboardSlot, Input::InputButtonType::KEY_ENTER))
            {
                Viewport::ClearColor(0, 0, 255);
            }
        }
        
        Display::FinishRender(0);
        Display::PresentAll();
        
    }
    TeaPacket::DeInitialize();
}
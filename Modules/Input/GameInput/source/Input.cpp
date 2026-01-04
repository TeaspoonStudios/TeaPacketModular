#include "TeaPacket/Input/Input.hpp"

#include <GameInput.h>

#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

#include "TeaPacket/Input/GameInputGlobal.hpp"
#include "TeaPacket/Input/InputDevice.hpp"
#include "TeaPacket/Input/InputFunctions.hpp"
#include "TeaPacket/Input/PlatformInputDevice.hpp"
#include "TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Input;

void Input::Initialize()
{
    CheckErrorWinCom(
        GameInputCreate(gameInput.GetAddressOf())
    );
    InitDeviceBackend();
}

void Input::DeInitialize()
{
    DeInitDeviceBackend();
    //gameInput->Release();
}
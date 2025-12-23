#include "TeaPacket/Input/InputDevice.hpp"

#include "GameInput.h"
#include "TeaPacket/Input/GameInputGlobal.hpp"
#include "TeaPacket/Input/InputButtons.hpp"
#include "TeaPacket/Input/PlatformInputDevice.hpp"
#include "TeaPacket/Input/InputButtonInfo/IsKeyboard.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

#include "TeaPacket/Input/GameInput/VirtualKey.gen"

using namespace TeaPacket::Input;

std::vector<InputDevice*> InputDevice::GetDevices()
{
    std::vector<InputDevice*> devices;
    devices.reserve(inputDevices.size());

    for (auto& device : inputDevices)
    {
        devices.emplace_back(&device);
    }

    return devices;
}

std::string_view InputDevice::GetName() const
{
    const GameInputDeviceInfo* info;
    CheckErrorWinCom(platformDevice->inputDevice->GetDeviceInfo(&info));
    return info->displayName == nullptr ? "NULL" : std::string_view(info->displayName);
}

void InputDevice::PollInput()
{
    CheckErrorWinCom(
        gameInput->GetCurrentReading(
            supportedInputTypes,
            platformDevice->inputDevice,
            platformDevice->reading.ReleaseAndGetAddressOf())
    );
    platformDevice->keyboardState.resize(platformDevice->reading->GetKeyCount());
    platformDevice->reading->GetKeyState(
        static_cast<uint32_t>(platformDevice->keyboardState.size()),
        platformDevice->keyboardState.data()
        );
    
}

bool InputDevice::GetButtonPressed(const InputButtonType button) const
{
    if (IsKeyboardButton(button))
    {
        const uint8_t virtualKey = InputButtonTypeToVK(button);
        for (const GameInputKeyState& keyState : platformDevice->keyboardState)
        {
            if (keyState.virtualKey == virtualKey)
            {
                return true;
            }
        }
        return false;
    }
    return false;
}


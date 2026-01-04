#include "TeaPacket/Input/InputFunctions.hpp"

#include <GameInput.h>

#include <wtypes.h>

#include "TeaPacket/Input/GameInputGlobal.hpp"
#include "TeaPacket/Input/InputAxis.hpp"
#include "TeaPacket/Input/InputButtonInfo.hpp"
#include "TeaPacket/Input/InputDevice.hpp"
#include "TeaPacket/Input/PlatformInputDevice.hpp"
#include "TeaPacket/Input/GameInput/VirtualKey.gen"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"
#include "TeaPacket/Window/PlatformWindow.hpp"
#include "TeaPacket/Window/Window.hpp"

using namespace TeaPacket;

static bool MouseGetButtonPressed(const InputDevice* device, const InputButtonType button)
{
    if (!IsMouseButton(button)) { return false; }
    const auto* state = std::any_cast<GameInputMouseState>(&device->controllerData);

    switch (button)
    {
        using enum InputButtonType;
    case MOUSE_LEFT:
        return state->buttons & GameInputMouseLeftButton;
    case MOUSE_RIGHT:
        return state->buttons & GameInputMouseRightButton;
    case MOUSE_MIDDLE:
        return state->buttons & GameInputMouseMiddleButton;
    default: return false;
    }
}

static void TP_GetWindowRect(RECT* rect)
{
    assert(rect != nullptr);
    assert(Window::Window::GetWindow(0) != nullptr);
    GetWindowRect(Window::Window::GetWindow(0)->platformWindow->windowHandle, rect);
}

static float MouseGetAxis(const InputDevice* device, const InputAxisType axis)
{
    const auto* state = std::any_cast<GameInputMouseState>(&device->controllerData);
    RECT windowRect;
    
    switch (axis)
    {
        using enum InputAxisType;
    case POINTER_X:
        TP_GetWindowRect(&windowRect);
        return static_cast<float>(state->absolutePositionX - windowRect.left) / (windowRect.right - windowRect.left);
    case POINTER_Y:
        TP_GetWindowRect(&windowRect);
        return static_cast<float>(state->absolutePositionY - windowRect.top) / (windowRect.bottom - windowRect.top);
    default:
        return std::numeric_limits<float>::infinity();
    }
}

static void MousePollInput(InputDevice* device)
{
    CheckErrorWinCom(
        gameInput->GetCurrentReading(
            GameInputKindMouse,
            device->platformDevice->inputDevice.Get(),
            device->platformDevice->reading.ReleaseAndGetAddressOf())
    );
    auto* state = std::any_cast<GameInputMouseState>(&device->controllerData);
    device->platformDevice->reading->GetMouseState(state);
}

InputDevice* Input::CreateMouseDevice()
{
    return &InputDevice::connectedDevices.v.emplace_front(InputDeviceParameters{
            .PollInputFunction = MousePollInput,
            .GetButtonFunction = MouseGetButtonPressed,
            .GetAxisFunction = MouseGetAxis,
            .GetNameFunction = GenericInputDeviceGetName,
            .isPhysical = true,
            .type = ControllerType::Mouse,
            .controllerData = GameInputMouseState()
        });
}

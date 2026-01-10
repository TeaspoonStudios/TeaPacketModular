#include "TeaPacket/Input/Input.hpp"

#include <vpad/input.h>

using namespace TeaPacket;
using namespace TeaPacket::Input;

#include "VPadCode.hpp"

void Input::Initialize()
{
    VPADInit();
}

void Input::DeInitialize()
{
    VPADShutdown();
}

void Input::UpdateControllers()
{
    
}

void Input::PollInputs(const ControllerSlot slot)
{
    if (slot == 0)
    {
        PollVPad();
    }
}

bool Input::IsConnected(const ControllerSlot slot)
{
    if (slot == 0)
    {
        return IsVPadConnected();
    }

    return false;
}

bool Input::IsButtonPressed(const ControllerSlot slot, const InputButtonType button)
{
    if (slot == 0)
    {
        return IsVPadButtonPressed(button);
    }
    return false;
}

bool Input::IsButtonSupported(const ControllerSlot slot, const InputButtonType button)
{
    if (slot == 0)
    {
        return IsVPadButtonSupported(button);
    }
    return false;
}

float Input::GetAxisValue(const ControllerSlot slot, const InputAxisType axis)
{
    if (slot == 0)
    {
        return GetVPadAxisValue(axis);
    }
    return 0;
}

ControllerSlot Input::GetSlotCount()
{
    return 5;
}

#include <TeaPacket/Logging/Logging.hpp>

#include "TeaPacket/Input/Input.hpp"
#include "TeaPacket/Types/Threading/SharedMutexPair.hpp"
#include "TeaPacket/Input/WiiU/VPadButtons.gen"

#include <vpad/input.h>

#include "TeaPacket/Input/InputAxis.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Input;

static VPADStatus currentVPadStatus;
static uint16_t vpadXTouch;
static uint16_t vpadYTouch;

static void PollVPad()
{
    VPADReadError err;
    while (true)
    {
        VPADRead(VPAD_CHAN_0, &currentVPadStatus, 1, &err);
        switch (err)
        {
        case VPAD_READ_SUCCESS:
            VPADGetTPCalibratedPointEx(VPAD_CHAN_0, VPAD_TP_1920X1080, &currentVPadStatus.tpNormal, &currentVPadStatus.tpNormal);
            if (!(currentVPadStatus.tpNormal.validity & VPAD_INVALID_X))
            {
                vpadXTouch = currentVPadStatus.tpNormal.x;
            }
            if (!(currentVPadStatus.tpNormal.validity & VPAD_INVALID_Y))
            {
                vpadYTouch = currentVPadStatus.tpNormal.y;
            }
        case VPAD_READ_NO_SAMPLES:
        case VPAD_READ_INVALID_CONTROLLER:
            return;
        case VPAD_READ_BUSY:
            continue;
        case VPAD_READ_UNINITIALIZED:
            throw std::runtime_error("VPAD Library not initialized");
        }
    }
}

static bool IsVPadConnected()
{
    VPADStatus dump;
    VPADReadError err;
    while (true)
    {
        VPADRead(VPAD_CHAN_0, &dump, 1, &err);
        switch (err)
        {
        case VPAD_READ_SUCCESS:
        case VPAD_READ_NO_SAMPLES:
            return true;
        case VPAD_READ_INVALID_CONTROLLER:
            return false;
        case VPAD_READ_BUSY:
            Log("UH OH");
            continue;
        case VPAD_READ_UNINITIALIZED:
            throw std::runtime_error("VPAD Library not initialized");
        }
    }
}

static bool IsVPadButtonPressed(const InputButtonType button)
{
    if (button == InputButtonType::MISC_TOUCH)
    {
        return currentVPadStatus.tpNormal.touched;
    }
    
    const VPADButtons vpadButton = InputButtonTypeToVPAD(button);
    return currentVPadStatus.hold & vpadButton;
}

static bool IsVPadButtonSupported(const InputButtonType button)
{
    return InputButtonTypeToVPAD(button) != VPAD_BUTTON_NONE;
}

static float GetVPadAxisValue(const InputAxisType axis)
{
    switch (axis)
    {
    case InputAxisType::POINTER_X:
        return static_cast<float>(vpadXTouch) / 1920.0f;
    case InputAxisType::POINTER_Y:
        return static_cast<float>(vpadYTouch) / 1080.0f;
    case InputAxisType::PAD_STICK_LEFT_X:
        return currentVPadStatus.leftStick.x;
    case InputAxisType::PAD_STICK_LEFT_Y:
        return currentVPadStatus.leftStick.y;
    case InputAxisType::PAD_STICK_RIGHT_X:
        return currentVPadStatus.rightStick.x;
    case InputAxisType::PAD_STICK_RIGHT_Y:
        return currentVPadStatus.rightStick.y;
    default: return 0;
    }
}

static bool GetVPadAxisSupported(const InputAxisType axis)
{
    switch (axis)
    {
    case InputAxisType::POINTER_X:
    case InputAxisType::POINTER_Y:
    case InputAxisType::PAD_STICK_LEFT_X:
    case InputAxisType::PAD_STICK_LEFT_Y:
    case InputAxisType::PAD_STICK_RIGHT_X:
    case InputAxisType::PAD_STICK_RIGHT_Y:
        return true;
    default: return false;
    }
}
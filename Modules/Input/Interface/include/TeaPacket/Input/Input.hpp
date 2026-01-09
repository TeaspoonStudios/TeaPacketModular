#pragma once
#include "TeaPacket/Core/Core.hpp"

#include <cstdint>

#include "TeaPacket/Input/ControllerType.hpp"

namespace TeaPacket::Input
{
    enum class InputButtonType : uint32_t;
    enum class InputAxisType : uint16_t;
    
    typedef uint8_t ControllerSlot;
    constexpr ControllerSlot NoControllerSlot = UINT8_MAX;
    
    void Initialize();
    void DeInitialize();

    /// Updates which controllers are currently connected in which slot.
    void UpdateControllers();

    /// Updates this slot's pressed buttons and axis values
    void PollInputs(ControllerSlot slot);

    /// Returns whether this slot has a connected controller
    bool IsConnected(ControllerSlot slot);

    /// Check Whether a button is pressed
    bool IsButtonPressed(ControllerSlot slot, InputButtonType button);

    float GetAxisValue(ControllerSlot slot, InputAxisType axis);

    /// Check whether a button is pressable on a slot.
    bool IsButtonSupported(ControllerSlot slot, InputButtonType button);

    bool IsAxisSupported(ControllerSlot slot, InputAxisType axis);

    ControllerSlot GetLastControllerPressed(ControllerType type = ControllerType::Any);

    std::string GetControllerName(ControllerSlot slot);
    
    /// Gets the amount of controller slots present. This is constant on most consoles but variable on mobile and PC.
    ControllerSlot GetSlotCount();


    inline void PollAllInputs()
    {
        const ControllerSlot slotCount = GetSlotCount();
        for (ControllerSlot slot = 0; slot < slotCount; slot++)
        {
            PollInputs(slot);
        }
    }
}
TP_HookInitDeInitFunctions(Input, 5);
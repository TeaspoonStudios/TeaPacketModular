#pragma once
#include "TeaPacket/Core/Core.hpp"

#include <cstdint>
#include <string>

#include "TeaPacket/Input/ControllerType.hpp"

/// A module for reading user input.
namespace TeaPacket::Input
{
    enum class InputButtonType : uint32_t;
    enum class InputAxisType : uint16_t;

    /// Represents a controller slot.
    typedef uint8_t ControllerSlot;
    /// Represents an invalid/no controller slot. Do not call any functions using this.
    constexpr ControllerSlot NoControllerSlot = UINT8_MAX;
    
    void Initialize();
    void DeInitialize();

    /// @brief Updates which controllers are currently connected in which slot.
    void UpdateControllers();

    /// @brief Updates this slot's pressed buttons and axis values
    /// @param slot The ControllerSlot to update.
    void PollInputs(ControllerSlot slot);

    /// @brief Returns whether this slot has a connected controller
    /// /// @param slot The ControllerSlot to check.
    /// @return Whether or not this slot has a connected controller.
    bool IsConnected(ControllerSlot slot);

    /// @brief Check Whether a button is pressed
    /// @param slot The ControllerSlot to check.
    /// @param button The button to check.
    /// @return Whether or not this button is pressed.
    bool IsButtonPressed(ControllerSlot slot, InputButtonType button);

    /// @brief Get the value of an axis.
    /// @param slot The ControllerSlot to check.
    /// @param axis The axis to get.
    /// @return The value of the axis. Will be 0 if this axis is not supported.
    float GetAxisValue(ControllerSlot slot, InputAxisType axis);

    /// @brief Check whether a button is pressable on a slot's controller..
    /// @param slot The ControllerSlot to check.
    /// @param button The button to check.
    /// @return Whether the button is supported or not.
    bool IsButtonSupported(ControllerSlot slot, InputButtonType button);

    /// @brief Check whether an axis exists on a slot's controller.
    /// @param slot The ControllerSlot to check.
    /// @param axis The axis to check.
    /// @return Whether the axis is supported or not.
    bool IsAxisSupported(ControllerSlot slot, InputAxisType axis);

    /// @brief Returns the last controller that has pressed a button.
    /// @param type Filters the controller by ControllerType. Only controllers with the same ControllerType will be considered. <br>
    /// Using ControllerType::Any will not filter out any controllers.
    /// @return The slot that last pressed a button.
    ControllerSlot GetLastControllerPressed(ControllerType type = ControllerType::Any);

    /// @brief Returns the name of a given controller.
    /// @param slot The slot to check.
    /// @return The name of the controller in that slot.
    std::string GetControllerName(ControllerSlot slot);
    
    /// @brief Gets the amount of controller slots present. This is constant on most consoles but variable on mobile and PC.
    ControllerSlot GetSlotCount();

    /// @brief Polls all controller slots.
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
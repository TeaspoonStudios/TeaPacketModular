#pragma once

#include <any>
#include <functional>
#include <string_view>

namespace TeaPacket::Input
{
    class InputDevice;
    enum class InputButtonType : uint32_t;
    enum class InputAxisType : uint16_t;
    enum class ControllerType : uint8_t;
    
    /// Parameters used for creating an InputDevice.
    struct InputDeviceParameters
    {
        std::function<void(InputDevice*)> PollInputFunction;
        std::function<bool(const InputDevice*, InputButtonType)> GetButtonFunction;
        std::function<float(const InputDevice*, InputAxisType)> GetAxisFunction;
        std::function<std::string_view(const InputDevice*)> GetNameFunction;

        bool isPhysical;
        ControllerType type;
        std::any controllerData;
    };
}

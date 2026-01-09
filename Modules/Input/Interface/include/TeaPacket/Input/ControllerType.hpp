#pragma once
#include <cstdint>

namespace TeaPacket::Input
{
    enum class ControllerType : uint8_t{
        None,
        Any,
        Keyboard,
        Mouse,
        Gamepad,
    };
}

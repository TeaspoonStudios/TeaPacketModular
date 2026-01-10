#pragma once
#include <cstdint>

#include "TeaPacket/Input/InputButtons.hpp"

namespace TeaPacket::Input
{
    /// Whether the given button is a keyboard button or not.
    constexpr bool IsKeyboardButton(const InputButtonType button)
    {
        return button > InputButtonType::START_KEY && button < InputButtonType::END_KEY;
    }

    /// Whether the given button is a mouse button or not.
    constexpr bool IsMouseButton(const InputButtonType button)
    {
        return button > InputButtonType::START_MOUSE && button < InputButtonType::END_MOUSE;
    }

    constexpr bool IsGamepadButton(const InputButtonType button)
    {
        return button > InputButtonType::START_PAD && button < InputButtonType::END_PAD;
    }
}

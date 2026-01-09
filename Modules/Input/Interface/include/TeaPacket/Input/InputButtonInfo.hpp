#pragma once
#include <cstdint>

#include "TeaPacket/Input/InputButtons.hpp"

namespace TeaPacket::Input
{
    constexpr bool IsKeyboardButton(const InputButtonType button)
    {
        return button > InputButtonType::START_KEY && button < InputButtonType::END_KEY;
    }

    constexpr bool IsMouseButton(const InputButtonType button)
    {
        return button > InputButtonType::START_MOUSE && button < InputButtonType::END_MOUSE;
    }
}

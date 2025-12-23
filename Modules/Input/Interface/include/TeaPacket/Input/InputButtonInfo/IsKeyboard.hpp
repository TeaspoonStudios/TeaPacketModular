#pragma once

#include <cstdint>

#include "TeaPacket/Input/InputButtons.hpp"

namespace TeaPacket::Input
{
    
    constexpr bool IsKeyboardButton(const InputButtonType button)
    {
        switch (button)
        {
            using enum InputButtonType;
        case KEY_BACKTICK:
        case KEY_1:
        case KEY_2:
        case KEY_3:
        case KEY_4:
        case KEY_5:
        case KEY_6:
        case KEY_7:
        case KEY_8:
        case KEY_9:
        case KEY_0:
        case KEY_MINUS:
        case KEY_EQUALS:
        case KEY_BACKSPACE:
        case KEY_TAB:
        case KEY_Q:
        case KEY_W:
        case KEY_E:
        case KEY_R:
        case KEY_T:
        case KEY_Y:
        case KEY_U:
        case KEY_I:
        case KEY_O:
        case KEY_P:
        case KEY_LEFT_SQUARE_BRACKET:
        case KEY_RIGHT_SQUARE_BRACKET:
        case KEY_BACKSLASH:
        case KEY_A:
        case KEY_S:
        case KEY_D:
        case KEY_F:
        case KEY_G:
        case KEY_H:
        case KEY_J:
        case KEY_K:
        case KEY_L:
        case KEY_SEMICOLON:
        case KEY_APOSTROPHE:
        case KEY_ENTER:
        case KEY_LEFT_SHIFT:
        case KEY_Z:
        case KEY_X:
        case KEY_C:
        case KEY_V:
        case KEY_B:
        case KEY_N:
        case KEY_M:
        case KEY_COMMA:
        case KEY_PERIOD:
        case KEY_FORWARD_SLASH:
        case KEY_RIGHT_SHIFT:
        case KEY_LEFT_CTRL:
        case KEY_WIN:
        case KEY_LEFT_ALT:
        case KEY_SPACE:
        case KEY_RIGHT_ALT:
        case KEY_RIGHT_CTRL:
        case KEY_ARROW_UP:
        case KEY_ARROW_RIGHT:
        case KEY_ARROW_DOWN:
        case KEY_ARROW_LEFT:
            return true;
        default:
            return false;
        }
    }
}

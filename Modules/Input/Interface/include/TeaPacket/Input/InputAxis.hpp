#pragma once
#include <cstdint>

namespace TeaPacket::Input
{
    /// Represents an axis on a controller. Not all controllers support all axes.
    enum class InputAxisType : uint16_t{
        POINTER_X, ///< A generic POINTER X axis. Depending on the device, this can represent mouse position, touch position, or pointer position.
        POINTER_Y, ///< Same as POINTER_X but for the Y axis.
        START_PAD,
        PAD_STICK_LEFT_X, ///< Left stick x on a controller.
        PAD_STICK_LEFT_Y, ///< Left stick y on a controller.
        PAD_STICK_RIGHT_X, ///< Right stick x on a controller.
        PAD_STICK_RIGHT_Y, ///< Right stick y on a controller.
        END_PAD
    };
}

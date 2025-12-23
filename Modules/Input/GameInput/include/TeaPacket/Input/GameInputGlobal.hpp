#pragma once

#include <GameInput.h>
#include <wrl/client.h>

#include "TeaPacket/Input/InputDevice.hpp"

#include <list>

using namespace GameInput::v3;

namespace TeaPacket::Input
{
    inline Microsoft::WRL::ComPtr<IGameInput> gameInput;
    inline std::list<InputDevice> inputDevices;

    constexpr GameInputKind supportedInputTypes =
        GameInputKindGamepad |
        GameInputKindKeyboard |
        GameInputKindMouse;
}

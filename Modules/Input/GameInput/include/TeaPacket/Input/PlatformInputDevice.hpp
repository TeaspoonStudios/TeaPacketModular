#pragma once

#include <wrl/client.h>
#include <GameInput.h>

#include <vector>

namespace TeaPacket::Input
{
    struct PlatformInputDevice
    {
        IGameInputDevice* inputDevice;
        Microsoft::WRL::ComPtr<IGameInputReading> reading;

        std::vector<GameInputKeyState> keyboardState;
    };
}

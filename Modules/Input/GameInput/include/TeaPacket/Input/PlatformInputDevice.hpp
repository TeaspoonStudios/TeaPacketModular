#pragma once

#include <wrl/client.h>
#include <GameInput.h>

using namespace GameInput::v3;

namespace TeaPacket::Input
{
    struct PlatformInputDevice
    {
        Microsoft::WRL::ComPtr<IGameInputDevice> inputDevice;
        Microsoft::WRL::ComPtr<IGameInputReading> reading;
    };
}

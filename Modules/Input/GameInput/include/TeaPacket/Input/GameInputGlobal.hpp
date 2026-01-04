#pragma once

#include <GameInput.h>
#include <unordered_map>
#include <wrl/client.h>

#include "TeaPacket/Types/Threading/SharedMutexPair.hpp"


using namespace GameInput::v3;

namespace TeaPacket::Input
{
    enum class ControllerType : uint8_t;
    inline Microsoft::WRL::ComPtr<IGameInput> gameInput;

    inline SharedMutexPair<std::unordered_map<IGameInputDevice*, ControllerType>> connectedGameInputDevices;
    constexpr auto GameInputKindAny = static_cast<GameInputKind>(0xFFFFFFFF);

    
    void InitDeviceBackend();
    void DeInitDeviceBackend();
}

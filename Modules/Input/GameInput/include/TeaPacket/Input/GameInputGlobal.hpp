#pragma once

#include <GameInput.h>
#include <wrl/client.h>

#include "TeaPacket/Types/Threading/SharedMutexPair.hpp"

#undef max // stupid ass windows header
using namespace GameInput::v3;

namespace TeaPacket::Input
{
    inline Microsoft::WRL::ComPtr<IGameInput> gameInput;

    constexpr auto GameInputKindAny = static_cast<GameInputKind>(std::numeric_limits<std::underlying_type_t<GameInputKind>>::max());
}

#pragma once
#include <atomic>

#include "TeaPacket/Core/Core.hpp"


namespace TeaPacket::System
{
    /// Initialize any core system-based functionality. Should be called first.
    void Initialize();
    /// De-Initialize any core system-based functionality. Should be called last.
    void DeInitialize();
    /// Process any system-based events, data, behaviors, etc.
    void ProcessSystem();

    void inline HookInits()
    {
        HookInitFunction(Initialize, 0);
    }

    /// Whether the app should be running or not. Will be false if the user requests to quit.
    bool ShouldRun();
}
TP_HookInitDeInitFunctions(System, 0);
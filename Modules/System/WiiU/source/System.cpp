#include "TeaPacket/System/System.hpp"

#include <whb/proc.h>

using namespace TeaPacket;

void System::Initialize()
{
    WHBProcInit();
}

void System::DeInitialize()
{
    WHBProcShutdown();
}

void System::ProcessSystem()
{
    isRunning = WHBProcIsRunning();
}

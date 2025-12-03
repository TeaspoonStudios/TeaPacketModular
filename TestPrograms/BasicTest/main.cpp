#include <iostream>

#include "TeaPacket/Logging/Logging.hpp"
#include "TeaPacket/System/System.hpp"

using namespace TeaPacket;
int main()
{
    System::Initialize();
    Logging::Initialize();

    while (System::isRunning)
    {
        LogString("HELLO WORLD!");
    }

    Logging::DeInitialize();
    System::DeInitialize();

    return 1;
}
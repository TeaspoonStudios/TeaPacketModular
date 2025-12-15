#include <iostream>

#include "TeaPacket/Assets/ReadAsset.hpp"
#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket;

int main()
{
    System::Initialize();
    Logging::Initialize();
    

    while (System::isRunning)
    {
        System::ProcessSystem();
        LogString(Assets::ReadTextFile("test.txt"));
        LogString(Assets::ReadTextFile("dir/test2.txt"));
    }

    Logging::DeInitialize();
    System::DeInitialize();
}
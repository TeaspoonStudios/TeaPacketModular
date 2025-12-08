#include <iostream>

#include "TeaPacket/Assets/ReadAsset.hpp"
#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket;

[[noreturn]] int main()
{
    System::Initialize();
    
    LogString(Assets::ReadTextFile("test.txt"));
    LogString(Assets::ReadTextFile("dir/test2.txt");

    System::DeInitialize();
}
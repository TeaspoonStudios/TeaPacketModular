#include <iostream>

#include "TeaPacket/Assets/ReadAsset.hpp"

using namespace TeaPacket::Assets;

[[noreturn]] int main()
{
    std::cout << ReadTextFile("test.txt") << std::endl;
    std::cout << ReadTextFile("dir/test2.txt") << std::endl;
}
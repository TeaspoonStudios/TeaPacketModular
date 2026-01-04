#pragma once

#include <mutex>

namespace TeaPacket
{
    template<typename T>
    class MutexPair
    {
    public:
        T v;
        std::mutex m;
    };
}
#pragma once

#include <shared_mutex>

namespace TeaPacket
{
    template<typename T>
    class SharedMutexPair
    {
    public:
        T v;
        std::shared_mutex m;
    };
}
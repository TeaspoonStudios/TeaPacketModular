#pragma once

namespace TeaPacket::Random {
    float RandFloat();

    int RandIntRangeInclusive(int min, int max);
    unsigned int RandUIntRangeInclusive(unsigned int min, unsigned int max);


    inline float RandFloatExclusive(const float max) {
        return RandFloat() * max;
    }
}
#include "TeaPacket/Random/Random.hpp"

#include <random>

using namespace TeaPacket;

static std::random_device device;
static std::mt19937 gen(device());
static std::uniform_real_distribution<float> floatDist(0, 1);

float Random::RandFloat() {
    return floatDist(gen);
}

int Random::RandIntRangeInclusive(const int min, const int max) {
    std::uniform_int_distribution distribution(min ,max);
    return distribution(gen);
}

unsigned int Random::RandUIntRangeInclusive(const unsigned int min, const unsigned int max) {
    std::uniform_int_distribution distribution(min ,max);
    return distribution(gen);
}

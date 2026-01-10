#pragma once
#include <stdexcept>
#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch"
#elif defined(__GNUC__) || defined(__GNUG__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wswitch"
#elif defined(_MSC_VER)
#pragma warning( push )
#pragma warning( disable : 4063 )
#endif

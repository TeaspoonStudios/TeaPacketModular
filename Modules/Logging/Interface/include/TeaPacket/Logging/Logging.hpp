#pragma once
#include <cassert>
#include <string>

namespace TeaPacket
{
    void LogString(const std::string& msg);

    template<typename T>
    void Log(T msg)
    {
        LogString(static_cast<std::string>(msg));
    }

    // Numeric Prints
#define LogNumDef(type) \
    template<> inline void Log(const type msg) \
    { \
        LogString(std::to_string(msg));\
    }
    
    LogNumDef(signed char)
    LogNumDef(unsigned char)
    LogNumDef(short int)
    LogNumDef(unsigned short int)
    LogNumDef(int)
    LogNumDef(unsigned int)
    LogNumDef(long int)
    LogNumDef(unsigned long int)
    LogNumDef(long long int)
    LogNumDef(unsigned long long int)
    LogNumDef(float)
    LogNumDef(double)
    LogNumDef(long double)

    template<> inline void Log(const bool msg)
    {
        LogString(msg ? "true" : "false");
    }

    template<> inline void Log(const char msg)
    {
        LogString(std::string(1,msg));
    }

    template<> inline void Log(const char* msg)
    {
        assert(msg != nullptr);
        LogString(std::string(msg));
    }
    
    
}


#undef LogNumDef
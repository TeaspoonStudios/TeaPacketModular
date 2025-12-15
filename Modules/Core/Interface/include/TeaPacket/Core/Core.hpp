#pragma once

#include <cstdint>
#include <functional>
#include <list>
#include <ranges>

namespace TeaPacket
{
    typedef std::function<void()> TP_InitFunction;
    typedef std::function<void()> TP_DeInitFunction;

    inline auto InitFunctionList = std::list<std::pair<TP_InitFunction, uint16_t>>();
    inline auto DeInitFunctionList = std::list<std::pair<TP_DeInitFunction, uint16_t>>();

    /// @brief Initializes all used TeaPacket systems
    inline void Initialize()
    {
        for (const auto& function : InitFunctionList | std::views::keys)
        {
            function();
        }
    }

    /// @brief DeInitializes all used TeaPacket systems
    inline void DeInitialize()
    {
        for (const auto& function : DeInitFunctionList | std::views::keys | std::views::reverse)
        {
            function();
        }
    }
    

    /// @brief Hooks a function to be called in INIT
    /// @param initFunction The function to be called
    /// @param priority The priority of the function. Init functions are called from smallest (1) to largest (100).
    inline void HookInitFunction(const TP_InitFunction& initFunction, const uint16_t priority)
    {
        for (auto i = InitFunctionList.begin(); i != InitFunctionList.end(); ++i)
        {
            if (i->second > priority)
            {
                InitFunctionList.insert(i, std::pair(
                        initFunction, priority
                    ));
                return;
            }
        }
        InitFunctionList.emplace_back(
                initFunction, priority
            );
    }

    /// @brief Hooks a function to be called in INIT
    /// @param deInitFunction The function to be called
    /// @param priority The priority of the function. DeInit functions are called from largest (100) to smallest (1).
    inline void HookDeInitFunction(const TP_DeInitFunction& deInitFunction, uint16_t priority)
    {
        for (auto i = DeInitFunctionList.begin(); i != DeInitFunctionList.end(); ++i)
        {
            if (i->second > priority)
            {
                DeInitFunctionList.insert(i, std::pair(
                        deInitFunction, priority
                    ));
                return;
            }
        }
        DeInitFunctionList.emplace_back(
                deInitFunction, priority
            );
    }

}

#define TP_HookInitFunction(Namespace, Priority) \
class TP_##Namespace##InitFunctionClass {    \
public: \
TP_##Namespace##InitFunctionClass() \
{ \
TeaPacket::HookInitFunction(TeaPacket::Namespace::Initialize, Priority);\
}\
}; \
inline TP_##Namespace##InitFunctionClass TP_##Namespace##InitFunctionRunner = TP_##Namespace##InitFunctionClass()

#define TP_HookDeInitFunction(Namespace, Priority) \
class TP_##Namespace##DeInitFunctionClass {    \
public: \
TP_##Namespace##DeInitFunctionClass() \
{ \
TeaPacket::HookDeInitFunction(TeaPacket::Namespace::DeInitialize, Priority);\
}\
}; \
inline TP_##Namespace##DeInitFunctionClass TP_##Namespace##DeInitFunctionRunner = TP_##Namespace##DeInitFunctionClass()

#define TP_HookInitDeInitFunctions(Namespace, Priority) \
    TP_HookInitFunction(Namespace, Priority); \
    TP_HookDeInitFunction(Namespace, Priority)


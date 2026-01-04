#pragma once

#include <forward_list>
#include <string_view>
#include <memory>
#include <vector>

#include <functional>
#include <shared_mutex>

#include "TeaPacket/Input/InputButtons.hpp"
#include "TeaPacket/Input/InputDeviceParameters.hpp"

#include "TeaPacket/Types/Threading/SharedMutexPair.hpp"

namespace TeaPacket::Input
{
    struct PlatformInputDevice;
    enum class InputButtonType : uint32_t;
    enum class InputAxisType : uint16_t;

    enum class ControllerType : uint8_t
    {
        Keyboard = 0,
        Mouse = 1,
        COUNT
    };

    struct DeviceSearchFilter
    {
        ControllerType type = ControllerType::COUNT;

        DeviceSearchFilter(const ControllerType type) : type(type) {}
    };
    
    class InputDevice
    {
    private:
        std::function<void(InputDevice*)> PollInputFunction;
        std::function<bool(const InputDevice*, InputButtonType)> GetButtonFunction;
        std::function<float(const InputDevice*, InputAxisType)> GetAxisFunction;
        std::function<std::string_view(const InputDevice*)> GetNameFunction;

        ControllerType type;
    
    public:
        explicit InputDevice(const InputDeviceParameters& parameters);
        ~InputDevice();
        
        std::unique_ptr<PlatformInputDevice> platformDevice;
        std::any controllerData;

        [[nodiscard]] std::string_view GetName() const
        {
            return GetNameFunction(this);
        }

        void PollInput()
        {
            PollInputFunction(this);
        }
        [[nodiscard]] bool GetButtonPressed(const InputButtonType button) const
        {
            return GetButtonFunction(this, button);
        }
        [[nodiscard]] float GetAxis(const InputAxisType axis) const
        {
            return GetAxisFunction(this, axis);
        }
        [[nodiscard]] ControllerType GetType() const
        {
            return type;
        }


    public:

        static inline SharedMutexPair<std::forward_list<InputDevice>> connectedDevices;

        static std::vector<InputDevice*> GetConnectedDevices()
        {
            std::shared_lock lock(connectedDevices.m);
            auto vec = std::vector<InputDevice*>();
            for (auto& device : connectedDevices.v)
            {
                vec.emplace_back(&device);
            }
            return vec;
        }
        static void UpdateConnectedDevices();

        static InputDevice* GetLastInputtedDevice(DeviceSearchFilter searchFilter);
        
        [[nodiscard]] static bool AnyDeviceHasButtonPressed(const InputButtonType button)
        {
            std::shared_lock lock(connectedDevices.m);
            for (const auto & device : connectedDevices.v)
            {
                if (device.GetButtonPressed(button))
                {
                    return true;
                }
            }
            return false;
        }
        
        static void PollAllDevices()
        {
            std::shared_lock lock(connectedDevices.m);
            for (auto & device : connectedDevices.v)
            {
                device.PollInput();
            }
        }
    };
}

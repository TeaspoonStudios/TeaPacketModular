#pragma once

#include <string_view>
#include <memory>
#include <vector>

namespace TeaPacket::Input
{
    struct PlatformInputDevice;
    enum class InputButtonType : uint32_t;
    
    class InputDevice
    {
    private:
    public:
        std::unique_ptr<PlatformInputDevice> platformDevice;

        [[nodiscard]] std::string_view GetName() const;

        void PollInput();
        [[nodiscard]] bool GetButtonPressed(InputButtonType button) const;

    public:

        static std::vector<InputDevice*> GetDevices();

        
        [[nodiscard]] static bool AnyDeviceHasButtonPressed(const InputButtonType button)
        {
            for (const auto & device : GetDevices())
            {
                if (device->GetButtonPressed(button))
                {
                    return true;
                }
            }
            return false;
        }
        static void PollAllDevices()
        {
            for (const auto & device : GetDevices())
            {
                device->PollInput();
            }
        }
    };
}
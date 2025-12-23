#include "TeaPacket/Input/Input.hpp"

#include <GameInput.h>

#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

#include "TeaPacket/Input/GameInputGlobal.hpp"
#include "TeaPacket/Input/InputDevice.hpp"
#include "TeaPacket/Input/PlatformInputDevice.hpp"
#include "TeaPacket/Logging/Logging.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Input;

void CALLBACK OnDeviceConnectedDisconnected(
    [[maybe_unused]] _In_ GameInputCallbackToken callbackToken,
    [[maybe_unused]] _In_ void * context,
    [[maybe_unused]] _In_ IGameInputDevice * device,
    [[maybe_unused]] _In_ uint64_t timestamp,
    [[maybe_unused]] _In_ GameInputDeviceStatus currentStatus,
    [[maybe_unused]] _In_ GameInputDeviceStatus previousStatus)
{
    const GameInputDeviceInfo* info = nullptr;
    CheckErrorWinCom(device->GetDeviceInfo(&info));
    
    if (currentStatus == GameInputDeviceConnected && previousStatus == GameInputDeviceNoStatus)
    {
        inputDevices.emplace_back(InputDevice{
            .platformDevice = std::make_unique<PlatformInputDevice>(PlatformInputDevice{
                .inputDevice = device
            })
        });
    } else if (currentStatus == GameInputDeviceNoStatus)
    {
        for (auto i = inputDevices.begin(); i != inputDevices.end(); ++i)
        {
            if (i->platformDevice->inputDevice == device)
            {
                inputDevices.erase(i);
                return;
            }
        }
    }
}

static GameInputCallbackToken callbackToken;

void Input::Initialize()
{
    CheckErrorWinCom(
        GameInputCreate(gameInput.GetAddressOf())
    );
    
    CheckErrorWinCom(gameInput->RegisterDeviceCallback(
        nullptr,
        supportedInputTypes,
        GameInputDeviceConnected,
        GameInputBlockingEnumeration,
        nullptr,
        OnDeviceConnectedDisconnected,
        &callbackToken
    ));

}

void Input::DeInitialize()
{
    gameInput->UnregisterCallback(callbackToken);
    //gameInput->Release();
}
#include "TeaPacket/Input/InputDevice.hpp"

#include <unordered_set>

#include "TeaPacket/Input/PlatformInputDevice.hpp"
#include "TeaPacket/Input/InputFunctions.hpp"
#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"
#include "TeaPacket/Input/GameInputGlobal.hpp"

using namespace TeaPacket::Input;
using namespace GameInput::v3;

constexpr static ControllerType controllerTypes[static_cast<size_t>(ControllerType::COUNT)] = {
    ControllerType::Keyboard,
    ControllerType::Mouse,
};

constexpr static GameInputKind GameInputKindFromControllerType(const ControllerType type)
{
    switch (type)
    {
        using enum ControllerType;
    case Keyboard: return GameInputKindKeyboard;
    case Mouse: return GameInputKindMouse;
    case COUNT: return GameInputKindAny;
    default: return GameInputKindUnknown;
    }
}

static GameInputCallbackToken callbackTokens[static_cast<size_t>(ControllerType::COUNT)] = {};

static void CALLBACK OnGameInputDeviceConnectedDisconnected(
    [[maybe_unused]] _In_ const GameInputCallbackToken callbackToken,
    [[maybe_unused]] _In_ void * context,
    [[maybe_unused]] _In_ IGameInputDevice * device,
    [[maybe_unused]] _In_ const uint64_t timestamp,
    [[maybe_unused]] _In_ const GameInputDeviceStatus currentStatus,
    [[maybe_unused]] _In_ const GameInputDeviceStatus previousStatus)
{
    const GameInputDeviceInfo* info = nullptr;
    CheckErrorWinCom(device->GetDeviceInfo(&info));

    const ControllerType controllerType = *static_cast<ControllerType*>(context);
    
    if (currentStatus == GameInputDeviceConnected && previousStatus == GameInputDeviceNoStatus)
    {
        // add device to connected devices list
        std::unique_lock lock(connectedGameInputDevices.m);
        if (!connectedGameInputDevices.v.contains(device)){
            connectedGameInputDevices.v[device] = controllerType;
        }
    } else if (currentStatus == GameInputDeviceNoStatus)
    {
        // remove device if needed from connected devices list
        std::unique_lock lock(connectedGameInputDevices.m);
        if (connectedGameInputDevices.v.contains(device))
        {
            connectedGameInputDevices.v.erase(device);
        }
    }
}

static void RegisterCallback(const GameInputKind kind, const ControllerType controllerType)
{
    CheckErrorWinCom(gameInput->RegisterDeviceCallback(
        nullptr,
        kind,
        GameInputDeviceAnyStatus,
        GameInputBlockingEnumeration,
        (void*)&controllerTypes[static_cast<size_t>(controllerType)],
        OnGameInputDeviceConnectedDisconnected,
        &callbackTokens[static_cast<size_t>(controllerType)]
    ));
}

void TeaPacket::Input::InitDeviceBackend()
{
    RegisterCallback(GameInputKindKeyboard, ControllerType::Keyboard);
    RegisterCallback(GameInputKindMouse, ControllerType::Mouse);
}

void TeaPacket::Input::DeInitDeviceBackend()
{
    for (const GameInputCallbackToken callbackToken : callbackTokens)
    {
        gameInput->UnregisterCallback(callbackToken);
    }
}

void InputDevice::UpdateConnectedDevices()
{
    std::shared_lock slock(connectedGameInputDevices.m, std::defer_lock);
    std::unique_lock ulock(connectedDevices.m, std::defer_lock);
    std::lock(ulock, slock);

    std::unordered_set<IGameInputDevice*> devicesRegistered;
    
    for (const auto& device : connectedDevices.v)
    {
        devicesRegistered.emplace(device.platformDevice->inputDevice.Get());
    }

    

    for (const auto& [device, type] : connectedGameInputDevices.v)
    {
        if (devicesRegistered.contains(device)) { continue; }
        InputDevice* inputDevice;

        switch (type)
        {
            using enum ControllerType;
        case Keyboard:
            inputDevice = CreateKeyboardDevice();
            break;
        case Mouse:
            inputDevice = CreateMouseDevice();
            break;
        default:
            throw std::runtime_error("NOT VALID DEVICE TYPE");
        }
        inputDevice->platformDevice = std::make_unique<PlatformInputDevice>(PlatformInputDevice{
            .inputDevice = device,
            .reading = nullptr,
        });
    }
}

InputDevice* InputDevice::GetLastInputtedDevice(DeviceSearchFilter searchFilter)
{
    Microsoft::WRL::ComPtr<IGameInputReading> reading;
    if (SUCCEEDED(gameInput->GetCurrentReading(
        GameInputKindFromControllerType(searchFilter.type),
        nullptr,
        reading.GetAddressOf()
    )))
    {
        IGameInputDevice* device;
        reading->GetDevice(&device);

        std::shared_lock lock(connectedDevices.m);
        for (auto& connectedDevice : connectedDevices.v)
        {
            if (connectedDevice.platformDevice->inputDevice.Get() == device)
            {
                return &connectedDevice;
            }
        }
    }
    return nullptr;
}

InputDevice::InputDevice(const InputDeviceParameters& parameters):
PollInputFunction(parameters.PollInputFunction),
GetButtonFunction(parameters.GetButtonFunction),
GetAxisFunction(parameters.GetAxisFunction),
GetNameFunction(parameters.GetNameFunction),
controllerData(parameters.controllerData)
{
    if (parameters.isPhysical) { return; }
}

std::string_view TeaPacket::Input::GenericInputDeviceGetName(const InputDevice* device)
{
    const GameInputDeviceInfo* info;
    CheckErrorWinCom(device->platformDevice->inputDevice->GetDeviceInfo(&info));
    return info->displayName == nullptr ? "NULL" : std::string_view(info->displayName);
}
InputDevice::~InputDevice() = default;
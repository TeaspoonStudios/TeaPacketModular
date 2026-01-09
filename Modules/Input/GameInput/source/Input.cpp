#include "TeaPacket/Input/Input.hpp"

#include <GameInput.h>

#include "TeaPacket/MacroUtils/WindowsSpecific.hpp"

#include "TeaPacket/Input/GameInputGlobal.hpp"
#include "TeaPacket/Input/InputAxis.hpp"
#include "TeaPacket/Input/InputButtonInfo.hpp"
#include "TeaPacket/Input/GameInput/VirtualKey.gen"
#include "TeaPacket/Input/GameInput/MouseButtons.gen"
#include "TeaPacket/Input/GameInput/ControllerType.gen"
#include "TeaPacket/Window/PlatformWindow.hpp"
#include "TeaPacket/Window/Window.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Input;

static GameInputCallbackToken globalCallbackToken;

struct ReadingEntry
{
    Microsoft::WRL::ComPtr<IGameInputReading> currentReading;
    Microsoft::WRL::ComPtr<IGameInputReading> lastReading;
};

static SharedMutexPair<std::vector<Microsoft::WRL::ComPtr<IGameInputDevice>>> connectedDevices;
static decltype(connectedDevices) publicDevices;
static SharedMutexPair<std::vector<ReadingEntry>> readings;

void CALLBACK OnDeviceEnumerated(
    [[maybe_unused]] _In_ GameInputCallbackToken callbackToken,
    [[maybe_unused]] _In_ void * context,
    [[maybe_unused]] _In_ IGameInputDevice * device,
    [[maybe_unused]] _In_ uint64_t timestamp,
    [[maybe_unused]] _In_ const GameInputDeviceStatus currentStatus,
    [[maybe_unused]] _In_ GameInputDeviceStatus previousStatus)
{
    std::unique_lock lock(connectedDevices.m);
    if (currentStatus & GameInputDeviceConnected)
    {
        // Connected
        for (auto& i : connectedDevices.v)
        {
            if (i == nullptr)
            {
                i = device;
                return;
            }
        }
        connectedDevices.v.emplace_back(device);
    } else
    {
        // Disconnected
        for (auto& i : connectedDevices.v)
        {
            if (i.Get() == device)
            {
                i = nullptr;
                return;
            }
        }
    }
}

void Input::UpdateControllers()
{
    std::unique_lock ulock(publicDevices.m, std::defer_lock);
    std::unique_lock ulock2(readings.m, std::defer_lock);
    std::shared_lock slock(connectedDevices.m, std::defer_lock);
    std::lock(ulock, ulock2, slock);

    publicDevices.v = connectedDevices.v;
    readings.v.resize(publicDevices.v.size());
}

bool Input::IsConnected(const ControllerSlot slot)
{
    std::shared_lock lock(connectedDevices.m);
    return connectedDevices.v[slot] != nullptr;
}

void Input::PollInputs(const ControllerSlot slot)
{
    std::shared_lock slock(publicDevices.m, std::defer_lock);
    std::unique_lock ulock(readings.m, std::defer_lock);
    std::lock(slock, ulock);
    // ReSharper disable once CppUseStructuredBinding
    auto& reading = readings.v[slot];
    reading.lastReading = reading.currentReading;
    
    if (!SUCCEEDED(gameInput->GetCurrentReading(
        GameInputKindAny,
        publicDevices.v[slot].Get(),
        reading.currentReading.GetAddressOf()
        )))
    {
        reading.currentReading = nullptr;
        return;
    }
}

void Input::Initialize()
{
    CheckErrorWinCom(
        GameInputCreate(gameInput.GetAddressOf())
    );

    CheckErrorWinCom(
        gameInput->RegisterDeviceCallback(
            nullptr,
            GameInputKindAny,
            GameInputDeviceConnected,
            GameInputAsyncEnumeration,
            nullptr,
            OnDeviceEnumerated,
            &globalCallbackToken)
    );
    
}

ControllerSlot Input::GetSlotCount()
{
    std::shared_lock lock(publicDevices.m);
    return static_cast<ControllerSlot>(publicDevices.v.size());
}

bool Input::IsButtonPressed(const ControllerSlot slot, const InputButtonType button)
{
    std::shared_lock lock(readings.m);
    IGameInputReading* reading = readings.v[slot].currentReading.Get();
    
    if (IsKeyboardButton(button))
    {
        const uint8_t virtualKey = InputButtonTypeToVK(button);
        const uint32_t keyCount = reading->GetKeyCount();
        auto keys = std::vector<GameInputKeyState>(keyCount);
        reading->GetKeyState(keyCount, keys.data());
        
        for (uint32_t i = 0; i < keyCount; i++)
        {
            if (keys[i].virtualKey == virtualKey)
            {
                return true;
            }
        }
        return false;
    } else if (IsMouseButton(button))
    {
        GameInputMouseState mouseState;
        reading->GetMouseState(&mouseState);
        return mouseState.buttons & InputButtonTypeToMouseButton(button);
    }

    return false;
}

static void TP_GetWindowRect(RECT* rect)
{
    assert(rect != nullptr);
    assert(Window::Window::Window::GetWindow(0) != nullptr);
    GetWindowRect(Window::Window::GetWindow(0)->platformWindow->windowHandle, rect);
}

float Input::GetAxisValue(const ControllerSlot slot, const InputAxisType axis)
{
    using enum InputAxisType;
    std::shared_lock lock(readings.m, std::defer_lock);
    std::shared_lock lock2(publicDevices.m, std::defer_lock);
    std::lock(lock, lock2);
    
    if (axis == POINTER_X || axis == POINTER_Y)
    {
        const GameInputDeviceInfo* info;
        if (!SUCCEEDED(publicDevices.v[slot]->GetDeviceInfo(&info)))
        {
            return 0;
        }
        if (info->supportedInput & GameInputKindMouse)
        {
            // POINTER is a mouse
            RECT windowRect;
            TP_GetWindowRect(&windowRect);
            GameInputMouseState mouseState;
            readings.v[slot].currentReading->GetMouseState(&mouseState);
            
            if (axis == POINTER_X)
            {
                return  static_cast<float>(mouseState.absolutePositionX - windowRect.left) /
                        static_cast<float>(windowRect.right - windowRect.left);
            } else
            {
                return  static_cast<float>(mouseState.absolutePositionY - windowRect.top) /
                        static_cast<float>(windowRect.bottom - windowRect.top);
            }
        }
    }

    return 0;
}


bool Input::IsButtonSupported(const ControllerSlot slot, const InputButtonType button)
{
    std::shared_lock lock(publicDevices.m);
    const GameInputDeviceInfo* info;
    const auto device = publicDevices.v[slot].Get();
    if (!SUCCEEDED(device->GetDeviceInfo(&info)))
    {
        return false;
    }
    if (IsKeyboardButton(button))
    {
        return info->supportedInput & GameInputKindKeyboard; // TODO: Is there a way to figure this out on a more granular level?
    } else if (IsMouseButton(button))
    {
        if (!(info->supportedInput & GameInputKindMouse))
        {
            return false;
        }
        return info->mouseInfo->supportedButtons & InputButtonTypeToMouseButton(button);
    }

    return false;
}

bool Input::IsAxisSupported(const ControllerSlot slot, const InputAxisType axis)
{
    std::shared_lock lock(publicDevices.m);
    const GameInputDeviceInfo* info;
    const auto device = publicDevices.v[slot].Get();
    if (!SUCCEEDED(device->GetDeviceInfo(&info)))
    {
        return false;
    }
    if (axis == InputAxisType::POINTER_X || axis == InputAxisType::POINTER_Y)
    {
        return info->supportedInput & GameInputKindMouse;
    }
    return false;
}


ControllerSlot Input::GetLastControllerPressed(const ControllerType type)
{
    Microsoft::WRL::ComPtr<IGameInputReading> reading;
    if (!SUCCEEDED(gameInput->GetCurrentReading(
        ControllerTypeToGameInputKind(type),
        nullptr,
        reading.GetAddressOf())))
    {
        return NoControllerSlot;
    }
    IGameInputDevice* device;
    reading->GetDevice(&device);
    
    std::shared_lock lock(publicDevices.m);
    for (size_t slot = 0; slot < publicDevices.v.size(); ++slot)
    {
        if (publicDevices.v[slot].Get() == device)
        {
            return static_cast<ControllerSlot>(slot);
        }
    }
    return NoControllerSlot;
}

std::string Input::GetControllerName(const ControllerSlot slot)
{
    const GameInputDeviceInfo* info;
    std::shared_lock lock(publicDevices.m);
    if (!SUCCEEDED(publicDevices.v[slot]->GetDeviceInfo(&info)))
    {
        return "";
    }
    return info->displayName;
}



void Input::DeInitialize()
{
    gameInput->UnregisterCallback(globalCallbackToken);
    //gameInput->Release();
}
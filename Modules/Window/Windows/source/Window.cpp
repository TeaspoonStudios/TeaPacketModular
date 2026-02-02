#include <memory>

#include "TeaPacket/Window/Window.hpp"

#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Window/PlatformWindow.hpp"

using namespace TeaPacket::Window;

static LRESULT WindowProc(const HWND hWnd, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
    switch (message)
    {
    case WM_DESTROY:
    case WM_QUIT:
        PostQuitMessage(0);
        // TODO: multiple windows on closing can be closed individually
        PlatformWindow::shouldQuit = true;
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}

static std::atomic_bool IsWindowClassInitialized(false);

static void InitializeWindowClass()
{
    IsWindowClassInitialized.store(true);

    WNDCLASS windowClass;
    windowClass.style = CS_VREDRAW | CS_HREDRAW;
    windowClass.lpfnWndProc = WindowProc;
    windowClass.cbClsExtra = 0;
    windowClass.cbWndExtra = 0;
    windowClass.hInstance = GetModuleHandle(NULL);
    windowClass.hIcon = 0;
    windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    windowClass.hbrBackground = CreateSolidBrush(RGB(29,29,38));
    windowClass.lpszMenuName = 0;
    windowClass.lpszClassName = "TeaPacket_MainWindowClass";

    if (!RegisterClass(&windowClass))
    {
        // Fatal Error
    }
}

Window::Window(const WindowParameters& parameters):
    platformWindow(std::make_unique<PlatformWindow>()),
    x(parameters.x),
    y(parameters.y),
    width(parameters.width),
    height(parameters.height),
    title(parameters.title),
    display(parameters.display)
{
    if (!IsWindowClassInitialized.load()) { InitializeWindowClass(); }

    const RECT rect = {0, 0, width, height};
    platformWindow->windowHandle = CreateWindow(
        "TeaPacket_MainWindowClass",
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        x, y,
        rect.right - rect.left, rect.bottom - rect.top,
        0,
        0,
        GetModuleHandle(NULL),
        0
    );

    if (platformWindow->windowHandle == NULL)
    {
        // Error handling
    }
    ShowWindow(platformWindow->windowHandle, SW_SHOWNORMAL);
    Windows.push_back(this);
}

#define UpdateWindowPosition() SetWindowPos( \
platformWindow->windowHandle, nullptr, \
x, y, 0, 0, \
SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE)

void Window::SetXPos(const unsigned short newXPos)
{
    x = newXPos;
    UpdateWindowPosition();
}

void Window::SetYPos(const unsigned short newYPos)
{
    y = newYPos;
    UpdateWindowPosition();
}

#define UpdateWindowSize() SetWindowPos(\
    platformWindow->windowHandle, nullptr, 0, 0, \
    width, height, \
    SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);

void Window::SetWidth(const unsigned short newWidth)
{
    width = newWidth;
    UpdateWindowSize();
}

void Window::SetHeight(const unsigned short newHeight)
{
    height = newHeight;
    UpdateWindowSize();
}

void Window::SetTitle(const std::string& newTitle)
{
    title = newTitle;
    SetWindowTextA(platformWindow->windowHandle, newTitle.c_str());
}

static thread_local MSG msg;

void Window::ProcessEvents()
{
    const HWND hwnd = platformWindow->windowHandle;
    while (PeekMessageA(&msg, hwnd, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

Window::~Window()
{
    for (size_t i = 0; i < Windows.size(); i++)
    {
        if (Windows[i] == this)
        {
            Windows.erase(Windows.begin() + i);
            break;
        }
    }
    DestroyWindow(platformWindow->windowHandle);

}
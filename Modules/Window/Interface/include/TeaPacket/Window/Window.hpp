#pragma once

#include <memory>
#include <string>
#include <vector>

#include "TeaPacket/Window/WindowParameters.hpp"

namespace TeaPacket::Graphics
{
    class Display;
}


namespace TeaPacket::Window
{
    struct PlatformWindow;
    /// @brief Represents one Window to the user. Does NOT represent the contained graphics.
    class Window
    {
    public:
        // Getters and Setters
        [[nodiscard]] auto GetXPos() const { return x; }
        void SetXPos(unsigned short newXPos);

        [[nodiscard]] auto GetYPos() const { return y; }
        void SetYPos(unsigned short newYPos);

        [[nodiscard]] auto GetHeight() const { return height; }
        void SetHeight(unsigned short newHeight);

        [[nodiscard]] auto GetWidth() const { return width; }
        void SetWidth(unsigned short newWidth);


        [[nodiscard]] const auto& GetTitle() const { return title; }
        void SetTitle(const std::string& newTitle);

        // Other Methods
        /// Processes all events that the window has received.
        void ProcessEvents();

        // Con/Destructor
        /// @brief Creates a Window using the given parameters, and make it visible to the user.
        /// @details Should not be called by users of the library directly. Create Graphics::Displays instead. \n
        /// Implementations of this function should:
        /// - Set this->params to a copy of parameters.
        /// - Initialize this->platformWindow with a new value.
        /// - Create and display a window visible to the end user, respecting all parameters given.
        /// - Add itself to Window::Windows.
        ///
        /// The appearance of the window without any associated Display is undefined and implementation-dependent.
        explicit Window(const WindowParameters& parameters);
        ~Window();

        /// Platform-Specific data.
        std::unique_ptr<PlatformWindow> platformWindow;
    private:

        unsigned short x;
        unsigned short y;
        unsigned short width;
        unsigned short height;

        std::string title;
        Graphics::Display* display;

    public:
        static [[nodiscard]] size_t GetWindowCount() { return Windows.size(); }
        static [[nodiscard]] Window* GetWindow(const size_t index) { return Windows[index]; }
    private:
        static inline std::vector<Window*> Windows = {};

    };
}
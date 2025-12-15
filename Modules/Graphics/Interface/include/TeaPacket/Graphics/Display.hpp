#pragma once
#include <vector>
#include <memory>

#include "TeaPacket/Graphics/Viewport.hpp"

namespace TeaPacket::Graphics{

    struct PlatformDisplay;
    struct DisplayParameters;

    /// @brief Represents a user-viewable area.
    /// @details On PC-based implementations, this typically represents a window.
    /// On non-PC implementations, this typically represents a screen.
    class Display{
        public:
        /// Gets the height of the Display.
        [[nodiscard]] uint16_t GetWidth() const {return viewport.GetWidth(); }
        /// Gets the width of the display.
        [[nodiscard]] uint16_t GetHeight() const { return viewport.GetHeight(); }

        /// Gets the viewport of this display.
        [[nodiscard]] Viewport* GetViewport() { return &viewport; }

        /// Constructs a display from parameters.
        explicit Display(const DisplayParameters& parameters);
        ~Display();

        /// A pointer to the implementation-defined data this display uses.
        std::unique_ptr<PlatformDisplay> platformDisplay;

        private:
        Viewport viewport;


    public:
        /// @brief Creates any number of displays, based on the platform.
        /// @details This function will always create at least 1 display.
        /// On PC platforms, this function will create a single window to be rendered to.
        /// On other platforms, this will create a Display for every screen that can be rendered to.
        static void InitializeDefaultDisplays(const std::vector<DisplayParameters>& requestedParameters);
        /// Gets the total number of created displays.
        [[nodiscard]] static size_t GetDisplayCount() { return Displays.size(); }
        /// Gets a display by index. Displays are given indices in ascending order.
        [[nodiscard]] static Display* GetDisplay(const unsigned char index) { return Displays[index].get(); }
        /// Destroys all Displays,
        static void DeInitialize();

        static void PresentAll();
        static void WaitForVSync();

    private:
        static inline std::vector<std::unique_ptr<Display>> Displays = {};
    };
}

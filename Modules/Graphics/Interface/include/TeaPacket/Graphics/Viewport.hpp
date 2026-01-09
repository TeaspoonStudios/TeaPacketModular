#pragma once
#include <memory>
#include "TeaPacket/Graphics/Texture/Texture.hpp"

namespace TeaPacket::Graphics
{
    struct PlatformViewport;
    struct ViewportParameters;
    class Display;

    /// Represents a render-able viewport. This is what graphics are drawn to.
    class Viewport
    {
    public:
        /// Gets the width of the viewport.
        [[nodiscard]] uint16_t GetWidth() const { return colorTexture.GetWidth(); }
        /// Gets the height of the viewport.
        [[nodiscard]] uint16_t GetHeight() const { return colorTexture.GetHeight(); }
        /// Gets the color texture associated with this viewport.
        [[nodiscard]] Texture* GetTexture() { return &colorTexture; }
        /// Gets the depth texture associated with this viewport.
        [[nodiscard]] Texture* GetDepthTexture() { return &depthTexture; }

        /// A pointer to the implementation-specific data this viewport uses.
        std::unique_ptr<PlatformViewport> platformViewport;

        /// Creates a viewport using parameters.
        explicit Viewport(const ViewportParameters& parameters);
        ~Viewport();

        /// Starts rendering to a viewport. This must be called before other draw calls.
        void BeginRender();
        /// Finishes rendering to a viewport. This must be called upon finishing rendering.
        /// @details This should always call Display::PresentDisplay() when this Viewport belongs to a Display.
        void FinishRender();

    private:
        Texture colorTexture;
        Texture depthTexture;

        Display* ownedDisplay;

    public:
        /// Clears the active viewport with a specific color.
        static void ClearColor(uint8_t r, uint8_t g, uint8_t b);

    private:
        static inline Viewport* activeViewport = nullptr;
    };
}

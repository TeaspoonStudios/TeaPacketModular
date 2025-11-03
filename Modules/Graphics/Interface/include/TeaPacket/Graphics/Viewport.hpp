#pragma once
#include <memory>
#include "TeaPacket/Graphics/Texture/Texture.hpp"

namespace TeaPacket::Graphics
{
    struct PlatformViewport;
    struct ViewportParameters;
    class Display;

    class Viewport
    {
    public:
        [[nodiscard]] uint16_t GetWidth() const { return colorTexture.GetWidth(); }
        [[nodiscard]] uint16_t GetHeight() const { return colorTexture.GetHeight(); }
        [[nodiscard]] Texture* GetTexture() { return &colorTexture; }
        [[nodiscard]] Texture* GetDepthTexture() { return &depthTexture; }

        std::unique_ptr<PlatformViewport> platformViewport;

        explicit Viewport(const ViewportParameters& parameters);
        ~Viewport();

        void BeginRender();
        void ClearColor(unsigned char r, unsigned char g, unsigned char b);
        void FinishRender();

    private:
        Texture colorTexture;
        Texture depthTexture;

        Display* ownedDisplay;

    private:
        static inline Viewport* activeViewport = nullptr;
    };
}

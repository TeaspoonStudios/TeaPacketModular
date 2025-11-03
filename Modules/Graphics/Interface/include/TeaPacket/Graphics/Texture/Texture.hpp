#pragma once
#include <memory>

namespace TeaPacket::Graphics
{
    struct PlatformTexture;
    struct TextureParameters;
    class TextureData;
    enum class TextureFormat : unsigned short;

    class Texture
    {
    public:
        [[nodiscard]] uint16_t GetWidth() const { return width; }
        [[nodiscard]] uint16_t GetHeight() const { return height; }

        std::unique_ptr<PlatformTexture> platformTexture;

        explicit Texture(const TextureParameters& parameters);
        ~Texture();

        [[nodiscard]] TextureData GetData() const;
    private:

        uint16_t width, height;
        TextureFormat format;
    };
}

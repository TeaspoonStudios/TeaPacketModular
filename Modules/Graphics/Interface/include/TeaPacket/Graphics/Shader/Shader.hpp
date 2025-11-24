#pragma once
#include <vector>
#include <memory>


namespace TeaPacket::Graphics
{
    struct VertexDataInfo;
    struct ShaderParameters;
    struct PlatformShader;

    class Shader
    {
    public: // Member Variables

        std::unique_ptr<PlatformShader> platformShader;
    private:
        std::vector<size_t> uniformBufferSizes{};
        
    public: // Static Variables
        static inline Shader* activeShader = nullptr;
    
        
    public: // Member Functions
        explicit Shader(const ShaderParameters& parameters);
        ~Shader();

        void SendUniformBuffer(const unsigned char* data, size_t bufferIndex);

        void SetActive();

    public: // Static Functions
    };
}
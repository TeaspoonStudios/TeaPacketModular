#pragma once
#include <string>

#include "TeaPacket/Graphics/Shader/ShaderVariable.hpp"

namespace TeaPacket
{
    enum class PrimitiveType : unsigned char;
    
}

namespace TeaPacket::Graphics
{


    
    struct ShaderFlags
    {
        
    };


    struct ShaderParameters
    {
        ShaderFlags flags;
        const std::string& vertexShaderCode, fragmentShaderCode;
        std::vector<ShaderVariableType>& inputAttributes;
        std::vector<size_t> uniformBufferSizes;
    };
}
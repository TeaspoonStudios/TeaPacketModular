#pragma once
#include <string>

#include "TeaPacket/Graphics/Shader/ShaderVariable.hpp"


namespace TeaPacket::Graphics
{


    /// The flags that a shader uses, in a standalone struct for packing purposes.
    struct ShaderFlags
    {
        
    };

    /// The parameters a shader takes on creation.
    struct ShaderParameters
    {
        /// The flags this shader will use.
        ShaderFlags flags;
        /// The vertex shader code this shader uses. Language is implementation-defined but use of AssetMixer is highly recommended.
        const std::string& vertexShaderCode;
        /// The fragment (pixel) shader code this shader uses. Language is implementation-defined but use of AssetMixer is highly recommended.
        const std::string&fragmentShaderCode;
        /// A list describing the vertex inputs this shader takes.
        const std::vector<ShaderVariableType>& inputAttributes;
    };
}
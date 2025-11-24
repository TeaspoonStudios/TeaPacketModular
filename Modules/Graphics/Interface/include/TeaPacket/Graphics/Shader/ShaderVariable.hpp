#pragma once

namespace TeaPacket::Graphics
{
    class Texture;
    
    enum class ShaderVariableBaseType : unsigned char
    {
        Float,  ///< Single-precision float
        Int,    ///< Signed Integer
        UInt,    ///< Unsigned Integer
    };

    union ShaderVariableValue
    {
        float f;
        signed int i;
        unsigned int ui;
    };

    struct ShaderVariableType
    {
        ShaderVariableBaseType baseType;
        unsigned char amount;

        constexpr ShaderVariableType():baseType(static_cast<ShaderVariableBaseType>(0)), amount(0) {}
        constexpr ShaderVariableType(const ShaderVariableBaseType type, const unsigned char amount) : baseType(type), amount(amount){}

        constexpr size_t GetSize() const;
    };
    
}
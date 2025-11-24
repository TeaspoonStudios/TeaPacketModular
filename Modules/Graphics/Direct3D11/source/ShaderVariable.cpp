#include "TeaPacket/Graphics/Shader/ShaderVariable.hpp"

#include <stdexcept>

#include "DirectXMath.h"

using namespace TeaPacket::Graphics;

constexpr size_t ShaderVariableType::GetSize() const
{
    switch (this->baseType)
    {
        using enum ShaderVariableBaseType;
    case Float:
        switch (this->amount)
        {
    case 1: return sizeof(float);
    case 2: return sizeof(DirectX::XMFLOAT2);
    case 3: return sizeof(DirectX::XMFLOAT3);
    case 4: return sizeof(DirectX::XMFLOAT4);
    default: break;
        }
    case Int:
        switch (this->amount)
        {
    case 1: return sizeof(int32_t);
    case 2: return sizeof(DirectX::XMINT2);
    case 3: return sizeof(DirectX::XMINT3);
    case 4: return sizeof(DirectX::XMINT4);
    default: break;
        }
    case UInt:
        switch (this->amount)
        {
    case 1: return sizeof(uint32_t);
    case 2: return sizeof(DirectX::XMUINT2);
    case 3: return sizeof(DirectX::XMUINT3);
    case 4: return sizeof(DirectX::XMUINT4);
    default: break;
        }
    }
    throw std::exception("BAD VALUE");
}

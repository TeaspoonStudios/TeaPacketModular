#pragma once
#include <cstdint>
#include <optional>
#include "TeaPacket/Types/Memory/BorrowedFixedArray.hpp"

namespace TeaPacket
{
    enum class PrimitiveType : unsigned char;
}

namespace TeaPacket::Graphics
{
    struct VertexDataInfo
    {
        PrimitiveType type;
        uint16_t size;
    };

    struct MeshFlags
    {
        bool useIndices;
    };

    struct MeshParameters
    {
        MeshFlags flags;
        BorrowedFixedArray<void> vertexData;
        std::vector<VertexDataInfo> vertexInfo;
        std::optional<BorrowedFixedArray<unsigned long>> indices;
    };
}
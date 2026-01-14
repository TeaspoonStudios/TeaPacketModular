#pragma once
#include <cstdint>
#include <optional>

#include "TeaPacket/Graphics/Shader/ShaderVariable.hpp"
#include "TeaPacket/Types/Memory/BorrowedFixedArray.hpp"

namespace TeaPacket::Graphics
{

    /// All the flags a mesh uses, grouped for packing purposes.
    struct MeshFlags
    {
        /// @brief Whether this mesh uses indices or not.
        /// @todo Do we need to keep this? Can this be replaced by just checking if indices is given?
        bool useIndices;
    };

    /// The parameters a mesh requires on creation.
    struct MeshParameters
    {
        /// The flags of the mesh.
        MeshFlags flags;
        /// A pointer to the vertex data this mesh uses.
        BorrowedFixedArray<void> vertexData;
        /// A list of VertexDataInfos that describe each vertex of the mesh.
        const std::vector<ShaderVariableType>& vertexInfo;
        /// An optional pointer to the face data this mesh uses.
        std::optional<BorrowedFixedArray<unsigned long>> indices;
    };
}
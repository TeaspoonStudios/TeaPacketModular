#pragma once
#include <memory>
#include <vector>

namespace TeaPacket::Graphics
{
    struct MeshParameters;
    struct PlatformMesh;
    struct ShaderVariableType;

    /// @brief Represents a reusable piece of geometry.
    /// @details A mesh represents only geometry, specifically a set of vertices and optionally a set of faces.
    class Mesh
    {
    public:

        /// A pointer to the implementation-defined data this mesh uses.
        std::unique_ptr<PlatformMesh> platformMesh;

        /// Whether or not the mesh uses Indices or not.
        const bool hasIndex : 1 = false;

    public:
        /// The currently active mesh that will be used when DrawMesh() is called.
        static inline Mesh* activeMesh = nullptr;

    private:
        /// A list describing how the vertices of this mesh are interpreted.
        std::vector<ShaderVariableType> vertexDataInfo;
        
    public:
        /// Constructs a mesh via parameters.
        Mesh() = default;
        explicit Mesh(const MeshParameters& parameters);
        ~Mesh();

        /// Sets this mesh to be the currently active mesh, to be used in future DrawMesh() calls.
        void SetActive();
    };
}

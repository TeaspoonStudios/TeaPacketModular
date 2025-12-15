#pragma once

#include "TeaPacket/Core/Core.hpp"

/// @brief A module for rendering and presenting graphics to the user.
/// @details This module specifically represents a shader-based, OpenGL style rendering engine.
/// Modules may support newer Graphics APIs, but any hardware lacking shader support will not be supported. <br>
/// This module may be renamed at a future date to a more descriptive name once fixed-function and Vulkan-style modules exist.
namespace TeaPacket::Graphics{
    /// Initializes the Graphics API. Must be called before any other Graphics functions.
    void Initialize();
    /// De-Initializes and cleans up the Graphics API.
    void DeInitialize();

    /// @brief Draws a mesh to the screen, using any and all active graphics data.
    /// @details
    /// This function heavily depends on several "active" variables/states. These are:
    /// - The active Mesh
    /// - The active Shader
    /// - The active Texture (s)
    /// - The data in each Uniform Buffer.
    /// - The active Viewport.
    void DrawMesh();
}
TP_HookInitDeInitFunctions(Graphics, 10);
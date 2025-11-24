#include <iostream>

#include "TeaPacket/Window/Window.hpp"
#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Mesh/Mesh.hpp"
#include "TeaPacket/Graphics/Mesh/MeshParameters.hpp"
#include "TeaPacket/Graphics/Shader/Shader.hpp"
#include "TeaPacket/Graphics/Shader/ShaderParameters.hpp"
#include "TeaPacket/Graphics/Texture/TextureData.hpp"
#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Types/Enums/PrimitiveTypes.hpp"
#include "TeaPacket/Assets/ReadAsset.hpp"
#include "TeaPacket/Graphics/Texture/TextureParameters.hpp"

using namespace TeaPacket;
using namespace TeaPacket::Window;
using namespace TeaPacket::Graphics;

constexpr float vertData[] = {
    -1.0f, 1.0f, 0.0f, 0.0f, 
     1.0f, 1.0f, 1.0f, 0.0f,
    -1.0f,-1.0f, 0.0f, 1.0f,
     1.0f,-1.0f, 1.0f, 1.0f
};

unsigned long faceData[] = {
    0, 1, 2,
    1, 3, 2
};

unsigned char texData[] = {
    255, 0, 0, 255,
    0, 255, 0, 255,
    0, 0, 255, 255,
    255, 255, 0, 255,
    0, 255, 255, 255,
    255, 0, 255, 255,
    0, 0, 0, 255,
    128, 128, 128, 255,
    255, 255, 255, 255
};

[[noreturn]] int main()
{
    Initialize();
    auto dispParams = DisplayParameters{.width = 1280, .height = 720};
    Display::InitializeDefaultDisplays({dispParams});
    Viewport* viewport = Display::GetDisplay(0)->GetViewport();

    auto vertInfo = std::vector<VertexDataInfo>(2);
    vertInfo[0].size = 2;
    vertInfo[0].type = PrimitiveType::Float;
    vertInfo[1].size = 2;
    vertInfo[1].type = PrimitiveType::Float;
    
    const auto meshParms = MeshParameters{
        .flags = MeshFlags{.useIndices = true},
        .vertexData = BorrowedFixedArray((void*)vertData, sizeof(vertData)),
        .vertexInfo = vertInfo,
        .indices = BorrowedFixedArray(faceData, std::size(faceData))
    };
    auto mesh = Mesh(meshParms);

    auto inputAttrs = std::vector<ShaderVariableType>(2);
    inputAttrs[0].baseType = ShaderVariableBaseType::Float;
    inputAttrs[0].amount = 2;
    inputAttrs[1].baseType = ShaderVariableBaseType::Float;
    inputAttrs[1].amount = 2;
    
    const auto shaderParms = ShaderParameters{
        .flags = {},
        .vertexShaderCode = Assets::ReadTextFile("test.vert"),
        .fragmentShaderCode = Assets::ReadTextFile("test.frag"),
        .inputAttributes = inputAttrs,
        .uniformBufferSizes = {16}
    };
    auto shader = Shader(shaderParms);
    constexpr float data[] = {0, 1, 0, 1};
    shader.SendUniformBuffer((unsigned char*)data, 0);

    const auto texParms = TextureParameters{
        .data = texData,
        .width = 3,
        .height = 3,
        .format = TextureFormat::RGBA8,
        .useFlags = TextureUseFlags{
            .shaderResource = true,
            .renderTargetColor = false,
            .renderTargetDepth = false,
            .writeMode = TextureAvailableMode::None,
            .cpuReadable = false
        }
    };
    auto tex = Texture(texParms);
    tex.SetActive(1);
    
    while (true)
    {
        System::ProcessSystem();
        viewport->BeginRender();
        viewport->ClearColor(255, 0, 0);

        mesh.SetActive();
        shader.SetActive();
        DrawMesh();
        
        viewport->FinishRender();
        static int number = 0;
        number++;
    }
    DeInitialize();
}
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

using namespace TeaPacket;
using namespace TeaPacket::Window;
using namespace TeaPacket::Graphics;

constexpr float vertData[] = {
    0.0f, 1.0f,
    1.0f,-1.0f,
    -1.0f,-1.0f
};

unsigned long faceData[] = {
    0, 1, 2
};

[[noreturn]] int main()
{
    Initialize();
    auto dispParams = DisplayParameters{.width = 1280, .height = 720};
    Display::InitializeDefaultDisplays({dispParams});
    Viewport* viewport = Display::GetDisplay(0)->GetViewport();

    auto vertInfo = std::vector<VertexDataInfo>(1);
    vertInfo[0].size = 2;
    vertInfo[0].type = PrimitiveType::Float;
    
    const auto meshParms = MeshParameters{
        .flags = MeshFlags{.useIndices = true},
        .vertexData = BorrowedFixedArray((void*)vertData, sizeof(vertData)),
        .vertexInfo = vertInfo,
        .indices = BorrowedFixedArray(faceData, 3)
    };
    auto mesh = Mesh(meshParms);

    auto inputAttrs = std::vector<ShaderVariableType>(1);
    inputAttrs[0].baseType = ShaderVariableBaseType::Float;
    inputAttrs[0].amount = 2;
    /*FixedArray<FixedArray<ShaderVariableType>> uniform{
        {
            ShaderVariableType(ShaderVariableBaseType::Float, 4),
            ShaderVariableType(ShaderVariableBaseType::Float, 2)
        } };*/
    
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
        if (number == 0)
        {
            auto texData = viewport->GetTexture()->GetData();
            const Color rgb = texData.GetColor4(1,1);
            std::cout << static_cast<std::string>(rgb);
        }
    }
    DeInitialize();
}
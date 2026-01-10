#include "TeaPacket/Graphics/Display.hpp"
#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Mesh/Mesh.hpp"
#include "TeaPacket/Graphics/Mesh/MeshParameters.hpp"
#include "TeaPacket/Graphics/Shader/Shader.hpp"
#include "TeaPacket/Graphics/Shader/ShaderParameters.hpp"
#include "TeaPacket/Graphics/Texture/TextureData.hpp"
#include "TeaPacket/System/System.hpp"
#include "TeaPacket/Assets/ReadAsset.hpp"
#include "TeaPacket/Graphics/Texture/TextureParameters.hpp"
#include "TeaPacket/Graphics/Shader/ShaderVariable.hpp"

#include "TeaPacket/Core/Core.hpp"
#include "TeaPacket/Graphics/UniformBuffer.hpp"
#include "TeaPacket/Graphics/UniformBufferParameters.hpp"

// Graphics Test Progress Stages
#define GTP_Initializable 0
#define GTP_CreateDisplays 1
#define GTP_PresentDisplays 2
#define GTP_ClearColor 3
#define GTP_ShadersAndMesh 4
#define GTP_Textures 5
#define GTP_Uniforms 6

#define GTP_STAGE GTP_ClearColor

using namespace TeaPacket;
using namespace TeaPacket::Graphics;

// Vertex Data W/O UV data
#if GTP_STAGE >= GTP_ShadersAndMesh && GTP_STAGE < GTP_Textures
constexpr float vertData[] = {
    -0.5f, 0.5f,
     0.5f, 0.5f,
    -0.5f,-0.5f,
     0.5f,-0.5f
};
#endif

// Vertex Data WITH UV Data
#if GTP_STAGE >= GTP_Textures
constexpr float vertData[] = {
    -0.5f, 0.5f, 0.0f, 1.0f, 
     0.5f, 0.5f, 1.0f, 1.0f,
    -0.5f,-0.5f, 0.0f, 0.0f,
     0.5f,-0.5f, 1.0f, 0.0f
};
#endif

// Face Data
#if GTP_STAGE >= GTP_ShadersAndMesh
unsigned long faceData[] = {
    0, 1, 2,
    1, 3, 2
};
#endif

// Texture Data
#if GTP_STAGE >= GTP_Textures
unsigned char texData[] = {
    255, 0, 0, 255, //red
    0, 255, 0, 255, //green
    0, 0, 255, 255, //blue
    255, 255, 0, 255, //yellow
    0, 255, 255, 255, // cyan
    255, 0, 255, 255, // magenta
    0, 0, 0, 255, // black
    128, 128, 128, 255, // grey
    255, 255, 255, 255 // white
};
#endif

int main()
{
    TeaPacket::Initialize();
#if GTP_STAGE >= GTP_CreateDisplays
    auto dispParams = DisplayParameters{.width = 1280, .height = 720};
    Display::InitializeDefaultDisplays({dispParams});
#endif
    
#if GTP_STAGE >= GTP_ShadersAndMesh

#if GTP_STAGE < GTP_Textures
    constexpr int shaderAttribCount = 1;
#else
    constexpr int shaderAttribCount = 2;
#endif
    
    
    auto vertInfo = std::vector<ShaderVariableType>(shaderAttribCount);
    vertInfo[0].amount = 2;
    vertInfo[0].baseType = ShaderVariableBaseType::Float;
#if GTP_STAGE >= GTP_Textures
    vertInfo[1].amount = 2;
    vertInfo[1].baseType = ShaderVariableBaseType::Float;
#endif
    
    
    const auto meshParms = MeshParameters{
        .flags = MeshFlags{.useIndices = true},
        .vertexData = BorrowedFixedArray((void*)vertData, sizeof(vertData)),
        .vertexInfo = vertInfo,
        .indices = BorrowedFixedArray(faceData, std::size(faceData))
    };
    auto mesh = Mesh(meshParms);

#if GTP_STAGE < GTP_Textures
    const auto shaderParms = ShaderParameters{
        .flags = {},
        .vertexShaderCode = Assets::ReadTextFile("color.vert"),
        .fragmentShaderCode = Assets::ReadTextFile("color.frag"),
        .inputAttributes = vertInfo,
    };
#else
    const auto shaderParms = ShaderParameters{
        .flags = {},
        .vertexShaderCode = Assets::ReadTextFile("textured.vert"),
        .fragmentShaderCode = Assets::ReadTextFile("textured.frag"),
        .inputAttributes = vertInfo,
    };
#endif
    
    auto shader = Shader(shaderParms);
#endif
#if GTP_STAGE >= GTP_Uniforms
    // WII U SPECIFIC
    // TODO: BufferHandler extension
    float data1[] = {1,1,1,1};

    for (float& val : data1)
    {
        UniformBuffer::SwapIfNeeded(val);
    }
    
    constexpr float data2[] = {0,0,0,0};
    const auto uniBufferParms = UniformBufferParameters{
        .data = (void*)data1,
        .size = sizeof(data1)
    };
    UniformBuffer uniformBuffer(uniBufferParms);
    uniformBuffer.SetActive(0);
    
#endif
    
#if GTP_STAGE >= GTP_Textures
    constexpr auto texParms = TextureParameters{
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
        },
        .filterMode = TextureFilterMode::Nearest,
        .wrapMode = TextureWrapMode::Wrap
    };
    auto tex = Texture(texParms);
    tex.SetActive(1);
#endif
    
    
    while (System::ShouldRun())
    {
        System::ProcessSystem();
#if GTP_STAGE >= GTP_PresentDisplays
        Display::BeginRender(0);
#if GTP_STAGE >= GTP_ClearColor
        static unsigned int i = 0;
        i = (i+1)%255;
        Viewport::ClearColor(static_cast<unsigned char>(i), 0, 0);
#endif

#if GTP_STAGE >= GTP_ShadersAndMesh
        mesh.SetActive();
        shader.SetActive();
#if GTP_STAGE >= GTP_Uniforms
        if (i > 100)
        {
            uniformBuffer.SendData(data2);
        } else
        {
            uniformBuffer.SendData(data1);
        }
#endif
        DrawMesh();
#endif
        
        Display::FinishRender(0);
        Display::PresentAll();
#endif
    }
    TeaPacket::DeInitialize();
}
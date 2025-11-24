#pragma once

#include <d3d11.h>
#include <wrl/client.h>

namespace TeaPacket::Graphics
{

    struct PlatformShader
    {
        Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
        Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;

        Microsoft::WRL::ComPtr<ID3D11InputLayout> inputLayout;

        std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> cbuffers;
    };
}
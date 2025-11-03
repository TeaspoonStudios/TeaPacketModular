#include <catch2/catch_test_macros.hpp>
#include "catch2/generators/catch_generators.hpp"
#include "catch2/generators/catch_generators_adapters.hpp"
#include "catch2/generators/catch_generators_random.hpp"

#include "TeaPacket/Graphics/Display.hpp"

#include <iostream>

#include "TeaPacket/Graphics/DisplayParameters.hpp"
#include "TeaPacket/Graphics/Graphics.hpp"
#include "TeaPacket/Graphics/Texture/TextureData.hpp"

using namespace TeaPacket::Graphics;

TEST_CASE("Display properties and Getters functional.", "[Graphics][Display]")
{
    Initialize();
    const uint16_t width = static_cast<uint16_t>(GENERATE(take(20, random(1,1920))));
    const uint16_t height = static_cast<uint16_t>(GENERATE(take(20, random(1,1080))));
    const auto params = std::vector{DisplayParameters{.width = width, .height = height}};
    Display::InitializeDefaultDisplays(params);
    Display* display = Display::GetDisplay(0);
    REQUIRE(display != nullptr);
    REQUIRE(display->GetHeight() == height);
    REQUIRE(display->GetWidth() == width);
    DeInitialize();
}

TEST_CASE("Ensure Viewport setup correctly", "[Graphics][Display]")
{
    Initialize();
    const uint16_t width = static_cast<uint16_t>(GENERATE(take(2, random(20,500))));
    const uint16_t height = static_cast<uint16_t>(GENERATE(take(2, random(20,500))));
    const auto params = std::vector{DisplayParameters{.width = width, .height = height}};
    Display::InitializeDefaultDisplays(params);
    Display* display = Display::GetDisplay(0);
    REQUIRE(display != nullptr);

    for (unsigned int i = 0; i < 5; i++)
    {
        display->GetViewport()->BeginRender();
        display->GetViewport()->ClearColor(255, 0, 0);
        display->GetViewport()->FinishRender();
    }

    TextureData texData = display->GetViewport()->GetTexture()->GetData();
    for (uint16_t y = 0; y < height; y++)
    {
        for (uint16_t x = 0; x < width; x++)
        {
            Color col = texData.GetColor4(x,y);
            REQUIRE(col.r() == 255);
            REQUIRE(col.g() == 0);
            REQUIRE(col.b() == 0);
            REQUIRE(col.a() == 255);
            //INFO("Check pixel" << x << " - " << y);
        }
    }

    DeInitialize();
}
#include "TeaPacket/Assets/ReadAsset.hpp"

#include <fstream>

using namespace TeaPacket;

static const auto assetPrepend = "assets/";


std::string Assets::ReadTextFile(const std::string& assetPath)
{
    std::string assetSource(assetPrepend);
    assetSource += assetPath;
    
    std::ifstream in(assetSource);
    
    in.seekg(0, std::ios::end);
    const size_t size = in.tellg();
    in.seekg(0);

    std::string data(size, '\0');
    in.read(&data[0], static_cast<std::streamsize>(size));
    return data;
}
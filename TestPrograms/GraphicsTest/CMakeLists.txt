cmake_minimum_required(VERSION 3.28)
project(TeaPacket_TestPrograms_Graphics)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")
include("${TEAPACKET_ROOT}/Subtools/AssetMixer/Functions.CMake")

add_executable(TeaPacket_TestPrograms_Graphics main.cpp)
TP_use_profile(Windows)
TP_AM_BuildAssets(
        TARGET TeaPacket_TestPrograms_Graphics 
        SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/" 
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
        ASSET_PLATFORM "STDFile"
        PARSER_DIRS "${TEAPACKET_ROOT}/Modules/Graphics/Direct3D11/AssetMixer/Parsers/"
        )
set_target_properties(TeaPacket_TestPrograms_Graphics PROPERTIES CXX_STANDARD 20)
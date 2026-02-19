cmake_minimum_required(VERSION 3.28)
project(TeaPacket_TestPrograms_Graphics)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")
include("${TEAPACKET_ROOT}/Subtools/AssetMixer/Functions.CMake")

add_executable(TeaPacket_TestPrograms_Graphics main.cpp)
TP_use_profile(${TEAPACKET_PLATFORM})
TP_add_extension_subdirectory(STBImageLoader)
target_link_libraries(TeaPacket_TestPrograms_Graphics TeaPacket_Extension_STBImageLoader)

TP_AM_BuildAssets(
        TARGET TeaPacket_TestPrograms_Graphics 
        SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/" 
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets/"
        PASSES ${TP_ASSET_MIXER_PASSES}
        )
set_target_properties(TeaPacket_TestPrograms_Graphics PROPERTIES CXX_STANDARD 20)
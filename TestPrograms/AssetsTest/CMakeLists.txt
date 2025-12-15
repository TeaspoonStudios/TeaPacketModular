cmake_minimum_required(VERSION 3.28)
project(TeaPacket_TestPrograms_Assets)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")
include("${TEAPACKET_ROOT}/Subtools/AssetMixer/Functions.CMake")

add_executable(TeaPacket_TestPrograms_Assets main.cpp)
TP_use_profile(${TEAPACKET_PLATFORM})
TP_AM_BuildAssets(
        TARGET TeaPacket_TestPrograms_Assets
        SOURCE "${CMAKE_CURRENT_SOURCE_DIR}/assets/"
        DESTINATION "${CMAKE_CURRENT_BINARY_DIR}"
        ASSET_PLATFORM "STDFile"
)
set_target_properties(TeaPacket_TestPrograms_Assets PROPERTIES CXX_STANDARD 20)
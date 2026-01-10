cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Input_WiiU)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

include("${TEAPACKET_ROOT}/Generators/EnumMapGen/Functions.CMake")

set(CMAKE_CXX_STANDARD 20)

TP_add_interface_subdirectory(Input)
TP_add_interface_subdirectory(Core)
TP_add_interface_subdirectory(Types)
TP_add_interface_subdirectory(Logging)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Input_WiiU STATIC ${sources})

TP_EnumMapGen("${CMAKE_CURRENT_SOURCE_DIR}/generators/VPadButton.gmap" TeaPacket_Input_WiiU)

target_include_directories(TeaPacket_Input_WiiU PRIVATE source)
target_include_directories(TeaPacket_Input_WiiU PUBLIC include)
target_link_libraries(TeaPacket_Input_WiiU TeaPacket_Input_Base 
        TeaPacket_Logging_Base TeaPacket_Core_Base
        TeaPacket_Types_Base
)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")
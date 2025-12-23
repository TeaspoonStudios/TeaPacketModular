cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Input_GameInput)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

include("${TEAPACKET_ROOT}/Generators/EnumMapGen/Functions.CMake")

set(CMAKE_CXX_STANDARD 20)

TP_add_interface_subdirectory(Input)
TP_add_interface_subdirectory(Core)
TP_add_interface_subdirectory(Logging)

add_subdirectory_no_overwrite(TeaPacket_MacroUtils_WinSpecific "${TEAPACKET_ROOT}/Modules/MacroUtils/WindowsSpecific" WindowsSpecificMacros)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Input_GameInput STATIC ${sources})

TP_EnumMapGen("${CMAKE_CURRENT_SOURCE_DIR}/generators/VirtualKey.gmap" TeaPacket_Input_GameInput)


target_include_directories(TeaPacket_Input_GameInput PRIVATE source)
target_include_directories(TeaPacket_Input_GameInput PUBLIC include 
"D:\\Downloads\\gameinput.3.1.26100.6879\\native\\include")
target_link_libraries(TeaPacket_Input_GameInput TeaPacket_Input_Base 
        TeaPacket_Logging_Base TeaPacket_Core_Base TeaPacket_MacroUtils_WinSpecific
        "D:\\Downloads\\gameinput.3.1.26100.6879\\native\\lib\\x64\\GameInput.lib"
)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")
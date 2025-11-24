cmake_minimum_required(VERSION 3.31)
project(TeaPacket_Graphics_Direct3D11)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

set(CMAKE_CXX_STANDARD 20)


TP_add_interface_subdirectory(Graphics)
TP_add_interface_subdirectory(MacroUtils)
TP_add_interface_subdirectory(Math)
add_subdirectory_no_overwrite(TeaPacket_MacroUtils_WinSpecific "${TEAPACKET_ROOT}/Modules/MacroUtils/WindowsSpecific" WindowsSpecificMacros)
TP_add_impl_subdirectory(Window Windows)
TP_add_impl_subdirectory(Logging STDIO)

file(GLOB_RECURSE sources source/*.c*)

add_library(TeaPacket_Graphics_Direct3D11 STATIC ${sources})
target_include_directories(TeaPacket_Graphics_Direct3D11 PRIVATE source)
target_include_directories(TeaPacket_Graphics_Direct3D11 PUBLIC include)
target_link_libraries(TeaPacket_Graphics_Direct3D11 TeaPacket_Graphics_Base
        TeaPacket_Window_Windows
        d3d11.lib D3DCompiler.lib
        TeaPacket_MacroUtils_Base TeaPacket_MacroUtils_WinSpecific
        TeaPacket_Math_Base TeaPacket_Logging_STDIO)

include("${TEAPACKET_ROOT}/CMakeCommons/CommonEnd.CMake")
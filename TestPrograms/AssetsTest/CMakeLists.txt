cmake_minimum_required(VERSION 3.31)
project(TeaPacket_TestPrograms_Assets)
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_impl_subdirectory(Assets STDFile)

add_executable(TeaPacket_TestPrograms_Assets main.cpp)
target_link_libraries(TeaPacket_TestPrograms_Assets PUBLIC
        TeaPacket_Assets_STDFile)
set_target_properties(TeaPacket_TestPrograms_Assets PROPERTIES CXX_STANDARD 20)
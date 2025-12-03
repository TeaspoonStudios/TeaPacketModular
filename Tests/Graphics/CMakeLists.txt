cmake_minimum_required(VERSION 3.28)
project(TeaPacket_Tests_Graphics_${TP_TEST_IMPL})
include("${TEAPACKET_ROOT}/CMakeCommons/CommonTest.CMake")
include("${TEAPACKET_ROOT}/CMakeCommons/CommonFunctions.CMake")

TP_add_impl_subdirectory(Graphics ${TP_TEST_IMPL})
TP_add_interface_subdirectory(Graphics)
TP_add_interface_subdirectory(System)

target_link_libraries(${PROJECT_NAME} INTERFACE TeaPacket_Graphics_Base)
target_link_libraries(${PROJECT_NAME} PUBLIC "TeaPacket_Graphics_${TP_TEST_IMPL}")
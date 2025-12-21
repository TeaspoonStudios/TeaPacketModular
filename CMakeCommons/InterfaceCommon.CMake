if(NOT DEFINED TP_DEBUG)
    set(TP_DEBUG 0)
endif()

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE incSources "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h*")

add_library(${PROJECT_NAME} INTERFACE ${incSources})
set_target_properties(${PROJECT_NAME} PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(${PROJECT_NAME} INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/include")
include("${CMAKE_CURRENT_LIST_DIR}/CommonEnd.CMake")
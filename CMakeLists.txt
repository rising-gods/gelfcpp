cmake_minimum_required(VERSION 3.6)
project(gelfcpp)

set(CMAKE_CXX_STANDARD 14)
include(cmake/EnableWarnings.cmake)

option(GELFCPP_BUILD_EXAMPLES "Build gelfcpp examples" OFF)
option(GELFCPP_WITH_COMPRESSION "Enable compression support" ON)

add_subdirectory(dep)

add_library(gelfcpp INTERFACE)
target_include_directories(gelfcpp INTERFACE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(gelfcpp INTERFACE rapidjson boost)
if (NOT GELFCPP_WITH_COMPRESSION)
    target_compile_definitions(gelfcpp INTERFACE -DGELFCPP_NO_COMPRESSION)
endif()

if (GELFCPP_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()

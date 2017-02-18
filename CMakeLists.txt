cmake_minimum_required(VERSION 3.5.1)
project(gelfcpp)

set(CMAKE_CXX_STANDARD 14)

# Subproject options
#set(GELFCPP_AS_SUBPROJECT       OFF                  CACHE BOOL   "Use gelfcpp as a subproject")
set(GELFCPP_BOOST_LIBRARY       "gelfcpp_boost"      CACHE STRING "Boost library used for gelfcpp")
set(GELFCPP_RAPIDJSON_LIBRARY   "gelfcpp_rapidjson"  CACHE STRING "rapidjson library used for gelfcpp")
set(GELFCPP_GOOGLETEST_LIBRARY  "gelfcpp_googletest" CACHE STRING "googletest library used for gelfcpp")

# Build options
if (NOT GELFCPP_AS_SUBPROJECT)
    option(GELFCPP_BUILD_EXAMPLES   "Build gelfcpp examples"     OFF)
    option(GELFCPP_BUILD_TESTS      "Build gelfcpp tests"         ON)
    option(GELFCPP_WITH_COMPRESSION "Enable compression support"  ON)
else()
    message(STATUS "gelfcpp: building as subproject")
    set(GELFCPP_BUILD_EXAMPLES  OFF)
    set(GELFCPP_BUILD_TESTS     OFF)
    if (NOT (DEFINED GELFCPP_WITH_COMPRESSION))
        set(GELFCPP_WITH_COMPRESSION ON)
    endif()
endif()
# Optional features

# Load dependencies
add_subdirectory(dep)

# Register library
add_library(gelfcpp INTERFACE)
target_include_directories(gelfcpp INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(gelfcpp INTERFACE ${GELFCPP_BOOST_LIBRARY} ${GELFCPP_RAPIDJSON_LIBRARY})
if (NOT GELFCPP_WITH_COMPRESSION)
    message(STATUS "gelfcpp: disabled compression support")
    target_compile_definitions(gelfcpp INTERFACE -DGELFCPP_NO_COMPRESSION)
endif()
if (NOT GELFCPP_AS_SUBPROJECT)
    target_compile_options(gelfcpp INTERFACE -Wall -Wextra -Wdouble-promotion)
endif()

# Additional build options
if (GELFCPP_BUILD_EXAMPLES)
    add_subdirectory(example)
endif()
if (GELFCPP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

find_package(RapidJSON)

add_library(gelfcpp_rapidjson INTERFACE)

if (RapidJSON_FOUND)
    message(STATUS "gelfcpp_rapidjson: Using System rapidjson")
    target_include_directories(gelfcpp_rapidjson INTERFACE ${RapidJSON_INCLUDE_DIRS})
else()
    # rapidjason: v1.1.0 (https://github.com/miloyip/rapidjson)
    message(STATUS "gelfcpp_rapidjson: Using local rapidjson")
    target_include_directories(gelfcpp_rapidjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/rapidjson/include)
endif()

target_compile_definitions(gelfcpp_rapidjson INTERFACE -DRAPIDJSON_HAS_STDSTRING=1)

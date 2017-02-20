# rapidjason: v1.1.0 (https://github.com/miloyip/rapidjson)
add_library(gelfcpp_rapidjson INTERFACE)
target_include_directories(gelfcpp_rapidjson INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(gelfcpp_rapidjson INTERFACE -DRAPIDJSON_HAS_STDSTRING=1)

add_subdirectory(boost)
add_subdirectory(rapidjson)

if (GELFCPP_BUILD_TESTS)
    add_subdirectory(googletest)
endif()

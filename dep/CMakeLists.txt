macro(add_dependency name skip_flag)
    if (${skip_flag} MATCHES "gelfcpp_.*")
        add_subdirectory(${name})
    endif()
endmacro()

add_dependency(boost GELFCPP_BOOST_LIBRARY)
add_dependency(rapidjson GELFCPP_RAPIDJSON_LIBRARY)

if (GELFCPP_BUILD_TESTS)
    add_subdirectory(googletest)
endif()

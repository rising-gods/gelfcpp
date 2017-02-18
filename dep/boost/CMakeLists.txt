if (GELFCPP_WITH_COMPRESSION)
    set(additional_libraries "iostreams")
endif()

find_package(Boost REQUIRED system ${additional_libraries})

add_library(gelfcpp_boost INTERFACE)
target_include_directories(gelfcpp_boost INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(gelfcpp_boost INTERFACE ${Boost_LIBRARIES})
target_compile_definitions(gelfcpp_boost INTERFACE -DBOOST_DATE_TIME_NO_LIB -DBOOST_REGEX_NO_LIB)

# googletest: release-1.8.0 (https://github.com/google/googletest/)

# Cleanup commands for source copy (adapted from https://github.com/llvm-mirror/llvm/tree/master/utils/unittest):
# both:
# $ rm -f CMakeLists.txt configure* Makefile* CHANGES CONTRIBUTORS README README.md .gitignore
# $ rm -f `find . -name \*\.pump`
# googletest:
# $ rm -rf build-aux cmake codegear m4 make msvc samples scripts test xcode docs
# googlemock:
# $ rm -rf build-aux make msvc scripts test docs

add_library(gelfcpp_googletest STATIC
        ${CMAKE_CURRENT_SOURCE_DIR}/googletest/src/gtest-all.cc
        ${CMAKE_CURRENT_SOURCE_DIR}/googlemock/src/gmock-all.cc)
target_include_directories(gelfcpp_googletest
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/googlemock/include
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/googletest
        ${CMAKE_CURRENT_SOURCE_DIR}/googlemock)
target_link_libraries(gelfcpp_googletest
        PUBLIC
        pthread)

add_library(gelfcpp_googletest_main STATIC
        googletest_main.cpp)
target_link_libraries(gelfcpp_googletest_main
        PUBLIC
        gelfcpp_googletest)

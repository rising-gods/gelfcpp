function(create_test name ...)
    message("Test: Adding '${name}' tests")

    set(name "gelfcpp_test_${name}")
    add_executable(${name} ${ARGV})
    target_link_libraries(${name} PUBLIC gelfcpp gelfcpp_googletest_main)
    add_test(${name} ${name})
endfunction()

function(create_all)
    message("Test: Adding 'all-in-one' test")

    file(GLOB_RECURSE sources ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    add_executable(gelfcpp_test_all ${sources})
    target_link_libraries(gelfcpp_test_all PUBLIC gelfcpp gelfcpp_googletest_main)
endfunction()

create_all()

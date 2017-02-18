function(create_test name ...)
    message("Test: Adding '${name}' tests")

    set(name "gelfcpp_test_${name}")
    add_executable(${name} ${ARGV})
    target_link_libraries(${name} PUBLIC gelfcpp googletest_main)
    add_test(${name} ${name})
endfunction()

create_test(message
        message/TestCompileGelfMessage.cpp)

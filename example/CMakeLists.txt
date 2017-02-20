function(add_example name)
    message("Example: Adding example '${name}'")

    set(name "gelfcpp_example_${name}")
    add_executable(${name} ${ARGN})
    target_link_libraries(${name} PUBLIC gelfcpp pthread)
endfunction()

add_example(default example_default.cpp)

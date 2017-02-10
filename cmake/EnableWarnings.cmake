# Enable compiler warnings
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wdouble-promotion")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wdouble-promotion")

# Enable in code static warnings
add_definitions(-DCONFIGURE_WITH_STATIC_WARNINGS)
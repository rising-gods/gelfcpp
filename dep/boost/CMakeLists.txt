if (GELFCPP_WITH_COMPRESSION)
    set(additional_libraries "iostreams")
endif()

find_package(Boost REQUIRED system ${additional_libraries})

# The Boost system and iostreams (binary) libraries
# depend on pthreads.
#
# Since Boost::system is not "required", we need
# to add Threads::Threads dependencies ourselves.
#
# See also https://stackoverflow.com/a/29871891
# For systems with multiple thread libraries (cmake < 3.14):
set(CMAKE_THREAD_PREFER_PTHREAD ON)
# The use of the -pthread compiler and linker flag is prefered:
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_library(gelfcpp_boost INTERFACE)
target_include_directories(gelfcpp_boost INTERFACE ${Boost_INCLUDE_DIRS})
target_link_libraries(gelfcpp_boost INTERFACE ${Boost_LIBRARIES} Threads::Threads)
target_compile_definitions(gelfcpp_boost INTERFACE -DBOOST_DATE_TIME_NO_LIB -DBOOST_REGEX_NO_LIB)

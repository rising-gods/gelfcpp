# gelfcpp
## Library for sending [Graylog Extended Log Format (GELF)](http://docs.graylog.org/en/stable/pages/gelf.html) messages from C++ applications.

* [gelfcpp GitHub](https://github.com/rising-gods/gelfcpp)
* [gelfcpp Documentation](https://rising-gods.github.io/gelfcpp/)

## Introduction
gelfcpp is a [Graylog Extended Log Format (GELF)](http://docs.graylog.org/en/stable/pages/gelf.html) interface for C++.
GELF is used by the open-source log management software [Graylog](https://www.graylog.org/).
It is able to send structured message log message into the system without the need to do expensive parsing.

## Requirements
* CMake >= 3.1
* C++14 compatible compiler
  * GCC >= 5.4.0
  * Clang >= 3.8.0
  * MSVC >= 14.2
* Boost
  * system
  * iostreams (optional, for compression)
* [rapidjson](https://github.com/miloyip/rapidjson) v1.1.0 (included)
* [googetest](https://github.com/google/googletest) v1.8.0 (included) (for tests only)
* [Doxygen](http://www.stack.nl/~dimitri/doxygen/) (for documentation only)

## Including gelfcpp in your projects
gelfcpp is header only and does not require building itself.

There are two methods for for including gelfcpp depending on your setup.
The recommended approach is [Variant 2: CMake subproject](#variant-2-cmake-subproject) and directly checking out this reposity into your project.
This way you can easily stay up to date with new releases!
### Variant 1: simple
1. Just copy everything in the [include/](include/) directory to your project and/or add gelfcpp to your include path.
2. Verify that you provide `rapidjson` and `Boost` in your setup.
3. Use it! See [Examples](#exampes).


### Variant 2: CMake subproject
1. Use the distribution as is (required `CMakeLists.txt`, `dep/`, `include/`)
2. Add **gelfcpp** via CMake:
```cmake
set(GELFCPP_AS_SUBPROJECT 1)
add_subdirectory(gelfcpp)
```
3. This includes already both dependencies.
4. Link your library/executable against the `gelfcpp` target in cmake:
```cmake
add_executable(my_cool_project main.cpp)
target_link_libraries(my_cool_project PUBLIC gelfcpp)
```
5. Use it! See [Examples](#exampes).


## Examples
See [Examples folder](example/) for further examples.

Sending messages to your Graylog instance.
```cpp
#include <gelfcpp/GelfMessageStream.hpp>
#include <gelfcpp/output/GelfUDPOutput.hpp>
#include <gelfcpp/decorator/Timestamp.hpp>

int main()
{
    gelfcpp::output::GelfUDPOutput graylog("graylog.example.com", 12001);
    gelfcpp::decorator::CurrentTimestamp timestamp;

    GELF_MESSAGE(graylog)
                    ("host", "localhost")
                    (timestamp)
                    ("I'm sending a message from gelfcpp")
                    ("awesomeness", 1337);
}
```

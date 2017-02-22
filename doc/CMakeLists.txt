find_package(Doxygen)

if (NOT DOXYGEN_FOUND)
    message(STATUS "Documentation: No doxygen found. Documentation will not be build.")
else()
    message(STATUS "Documentation: Found doxygen. Building documentation")

    file(GLOB_RECURSE sources ${CMAKE_SOURCE_DIR}/include/*)
    list(APPEND docs ${CMAKE_SOURCE_DIR}/README.md)

    configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    add_custom_command(OUTPUT doc_html
            COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/html
            COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            DEPENDS ${sources} ${docs} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
            WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
    add_custom_target(doc ALL DEPENDS doc_html)
endif()

# config
set(CMAKE_TOOLCHAIN_FILE "./tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
cmake_minimum_required(VERSION 3.20)
project(bika)
set(CMAKE_CXX_STANDARD 20)

# output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# 3rd-party
find_package(doctest CONFIG REQUIRED)

# library
add_subdirectory(bika/)

# test
add_executable(test_bika tests/test_bika.cpp)
target_link_libraries(test_bika PRIVATE sfl::bika doctest::doctest)
add_custom_command(
    TARGET test_bika POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config/config.yml
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yml)

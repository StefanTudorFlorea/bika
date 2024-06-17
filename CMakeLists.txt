# config
set(CMAKE_TOOLCHAIN_FILE "./tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
cmake_minimum_required(VERSION 3.20)
project(bika)
set(CMAKE_CXX_STANDARD 20)

# output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# library
add_subdirectory(bika/)

# testing
add_subdirectory(tests/)

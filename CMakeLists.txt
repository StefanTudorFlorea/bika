# config
set(APP app)

set(CMAKE_TOOLCHAIN_FILE "./tools/vcpkg/scripts/buildsystems/vcpkg.cmake")
cmake_minimum_required(VERSION 3.20)
project(${APP})
set(CMAKE_CXX_STANDARD 20)

# output
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 3rd party
find_package(Crow          CONFIG REQUIRED)
find_package(doctest       CONFIG REQUIRED)
find_package(fmt           CONFIG REQUIRED)
find_package(libpqxx       CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(yaml-cpp      CONFIG REQUIRED)
find_package(httplib       CONFIG REQUIRED)
find_package(bsoncxx       CONFIG REQUIRED)
find_package(mongocxx      CONFIG REQUIRED)

# services
add_subdirectory(src/common)
add_subdirectory(src/broker-service)
add_subdirectory(src/auth-service)
add_subdirectory(src/logger-service)
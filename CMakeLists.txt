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
find_package(doctest       CONFIG REQUIRED)
find_package(fmt           CONFIG REQUIRED)
find_package(libpqxx       CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(yaml-cpp      CONFIG REQUIRED)
find_package(httplib       CONFIG REQUIRED)

# backend
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp)
add_executable(backend ${SRC_FILES})
target_include_directories(backend PUBLIC . ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(backend 
    PRIVATE 
        httplib::httplib
        libpqxx::pqxx
        fmt::fmt-header-only
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
)

# config for all services
add_custom_command(
    TARGET backend POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config/backend.yml
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/backend.yml)
# config
project(common)

# common
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
add_library(common ${SRC_FILES})

target_link_libraries(common
    PUBLIC
        fmt::fmt-header-only
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
    PRIVATE
        httplib::httplib
)
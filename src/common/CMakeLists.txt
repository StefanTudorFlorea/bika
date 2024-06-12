# config
project(common)

# common
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
add_library(common ${SRC_FILES})

target_link_libraries(common
    PRIVATE
        fmt::fmt-header-only
        nlohmann_json::nlohmann_json
        httplib::httplib
)
# 3rd party
find_package(fmt           CONFIG REQUIRED)
find_package(libpqxx       CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(yaml-cpp      CONFIG REQUIRED)
find_package(httplib       CONFIG REQUIRED)
find_package(cpr           CONFIG REQUIRED)

# library
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS src/*.cpp include/*.h)
add_library(bika ${SRC_FILES})
target_include_directories(bika PUBLIC . include/)

# depedencies
target_link_libraries(bika 
    PUBLIC 
        httplib::httplib
        libpqxx::pqxx
        fmt::fmt-header-only
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
        cpr::cpr
)

# alias
add_library(sfl::bika ALIAS bika)
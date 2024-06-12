# auth-service
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
add_executable(auth-service ${SRC_FILES})
target_include_directories(auth-service PUBLIC . ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(auth-service 
    PRIVATE 
        fmt::fmt-header-only
        libpqxx::pqxx
        Crow::Crow
        nlohmann_json::nlohmann_json
        yaml-cpp::yaml-cpp
        httplib::httplib
        common
)

# config for all services
add_custom_command(
    TARGET auth-service POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config/services.yml
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/services.yml)
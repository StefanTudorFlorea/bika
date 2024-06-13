# logger-service
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
add_executable(logger-service ${SRC_FILES})
target_include_directories(logger-service PUBLIC . ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(logger-service 
    PRIVATE 
        common
        httplib::httplib
        mongo::mongocxx_static
        mongo::bsoncxx_static
)

# config for all services
add_custom_command(
    TARGET logger-service POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config/services.yml
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/services.yml)
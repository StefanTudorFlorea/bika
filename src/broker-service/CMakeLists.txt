# broker-service
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS *.cpp)
add_executable(broker-service ${SRC_FILES})
target_include_directories(broker-service PUBLIC . ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(broker-service 
    PRIVATE 
        common
        httplib::httplib
)

# config for all services
add_custom_command(
    TARGET broker-service POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config/services.yml
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/services.yml)
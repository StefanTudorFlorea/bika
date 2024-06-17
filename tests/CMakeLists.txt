find_package(doctest CONFIG REQUIRED)

file(GLOB_RECURSE TEST_SRC_FILES CONFIGURE_DEPENDS ../tests/*.cpp)
add_executable(test_bika ${TEST_SRC_FILES})

target_link_libraries(test_bika PRIVATE sfl::bika doctest::doctest)

add_custom_command(
    TARGET test_bika POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/config/config.yml
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.yml)

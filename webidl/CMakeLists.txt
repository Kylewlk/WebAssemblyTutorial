cmake_minimum_required(VERSION 3.21)
project(WebAssemblyTutorial-webidl)

set(CMAKE_EXECUTABLE_SUFFIX .html)

add_executable(webidl test.cpp test.h)

target_link_options(webidl PRIVATE "--post-js=${CMAKE_CURRENT_SOURCE_DIR}/glue.js"
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
    "-sEXPORTED_FUNCTIONS=[_malloc, _free]"
    "-sEXPORTED_RUNTIME_METHODS=[stringToUTF8]"
    # "--extern-post-js=${CMAKE_CURRENT_SOURCE_DIR}/post.js"
)

add_custom_command(
    TARGET webidl POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/post.js
    ${CMAKE_CURRENT_BINARY_DIR}/post.js
)

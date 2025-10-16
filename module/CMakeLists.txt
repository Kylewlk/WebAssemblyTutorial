cmake_minimum_required(VERSION 3.21)
project(WebAssemblyTutorial-module)

set(CMAKE_EXECUTABLE_SUFFIX .js)

add_executable(module module.cpp module.h)

target_link_options(module PRIVATE
    "--post-js=${CMAKE_CURRENT_BINARY_DIR}/module_glue.js"
    "--post-js=${CMAKE_CURRENT_SOURCE_DIR}/post.js"
    "--pre-js=${CMAKE_CURRENT_SOURCE_DIR}/pre.js"
    "-sMODULARIZE=1"
    "-sEXPORT_ES6=1"
    "-sEXPORT_NAME=Mym"
    "-sEXPORTED_RUNTIME_METHODS=[\"stringToUTF8\", \"addFunction\"]"
    "-sALLOW_TABLE_GROWTH=1"

    # "--extern-post-js=${CMAKE_CURRENT_SOURCE_DIR}/post.js"
)

target_include_directories(module PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# message(STATUS "webidl bin:${CMAKE_CURRENT_BINARY_DIR}")

set(PYTHON $ENV{EMSDK_PYTHON})
set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten)
set(WEBIDL_BINDER_SCRIPT ${EMSCRIPTEN_ROOT}/tools/webidl_binder.py)
set(WEB_IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/module.idl)

add_custom_command(
    OUTPUT module_glue.cpp module_glue.js
    COMMAND ${PYTHON} ${WEBIDL_BINDER_SCRIPT} ${WEB_IDL_FILE} module_glue
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${WEB_IDL_FILE}
    COMMENT "Generating module idl binding files"
    VERBATIM)

add_custom_target(module-bindings ALL DEPENDS module_glue.cpp)

add_dependencies(module module-bindings)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/module.html DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

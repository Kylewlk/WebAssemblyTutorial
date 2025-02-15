cmake_minimum_required(VERSION 3.21)
project(WebAssemblyTutorial-webidl)

set(CMAKE_EXECUTABLE_SUFFIX .html)

add_executable(webidl test.cpp test.h)

target_link_options(webidl PRIVATE "--post-js=${CMAKE_CURRENT_BINARY_DIR}/glue.js"
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
    "-sEXPORTED_FUNCTIONS=[\"_malloc\", \"_free\"]"
    "-sEXPORTED_RUNTIME_METHODS=[\"stringToUTF8\", \"addFunction\"]"
    "-sALLOW_TABLE_GROWTH=1"

    # "--extern-post-js=${CMAKE_CURRENT_SOURCE_DIR}/post.js"
)

target_include_directories(webidl PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

# message(STATUS "webidl bin:${CMAKE_CURRENT_BINARY_DIR}")

set(PYTHON $ENV{EMSDK_PYTHON})
set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten)
set(WEBIDL_BINDER_SCRIPT ${EMSCRIPTEN_ROOT}/tools/webidl_binder.py)
set(WEB_IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/test.idl)

add_custom_command(
    OUTPUT glue.cpp glue.js
    BYPRODUCTS parser.out WebIDLGrammar.pkl
    COMMAND ${PYTHON} ${WEBIDL_BINDER_SCRIPT} ${WEB_IDL_FILE} glue
    DEPENDS ${WEB_IDL_FILE}
    COMMENT "Generating web idl binding files"
    VERBATIM)
message(STATUS "${PYTHON} ${WEBIDL_BINDER_SCRIPT} ${WEB_IDL_FILE} glue")

add_custom_target(webidl-bindings ALL DEPENDS glue.cpp)

add_dependencies(webidl webidl-bindings)

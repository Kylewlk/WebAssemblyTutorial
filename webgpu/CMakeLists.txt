cmake_minimum_required(VERSION 3.21)
project(WebAssemblyTutorial-webgpu)

set(CMAKE_EXECUTABLE_SUFFIX .html)

add_executable(webgpu webgpu.cpp webgpu.h)

target_link_options(webgpu PRIVATE
    # "--post-js=${CMAKE_CURRENT_BINARY_DIR}/glue.js"
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
    # "-sEXPORTED_FUNCTIONS=[\"_malloc\", \"_free\"]"
    # "-sEXPORTED_RUNTIME_METHODS=[\"stringToUTF8\"]"
    # "--extern-post-js=${CMAKE_CURRENT_SOURCE_DIR}/post.js"
)

target_include_directories(webgpu PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set(PYTHON $ENV{EMSDK_PYTHON})
set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten)
set(WEBIDL_BINDER_SCRIPT ${EMSCRIPTEN_ROOT}/tools/webidl_binder.py)
set(WEB_IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/webgpu.idl)

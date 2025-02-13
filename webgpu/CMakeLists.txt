cmake_minimum_required(VERSION 3.21)
project(WebAssemblyTutorial-webgpu)

set(CMAKE_EXECUTABLE_SUFFIX .html)

add_executable(webgpu webgpu.cpp webgpu.h)

target_compile_options(webgpu PUBLIC
    # "-sUSE_PTHREADS=1"
    "-sWASM_WORKERS=1"
)

target_link_options(webgpu PRIVATE

    "--post-js=${CMAKE_CURRENT_BINARY_DIR}/webgpu_glue.js"
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell.html"

    # "-sEXPORTED_FUNCTIONS=[\"_malloc\", \"_free\"]"
    # "-sEXPORTED_RUNTIME_METHODS=[\"stringToUTF8\"]"
    # "--extern-post-js=${CMAKE_CURRENT_SOURCE_DIR}/post.js"
    "-sUSE_WEBGPU=1"
    "-sASYNCIFY=1"
    "-sFETCH=1"
    "-sFILESYSTEM=0"
    # "-sALLOW_MEMORY_GROWTH=1"
    "-sWASM_WORKERS=1"
    # "-sEXIT_RUNTIME=0"

    # "-sPTHREAD_POOL_SIZE=2"
    # "-sUSE_PTHREADS=1"
)

target_include_directories(webgpu PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

set(PYTHON $ENV{EMSDK_PYTHON})
set(EMSCRIPTEN_ROOT $ENV{EMSDK}/upstream/emscripten)
set(WEBIDL_BINDER_SCRIPT ${EMSCRIPTEN_ROOT}/tools/webidl_binder.py)
set(IDL_FILE ${CMAKE_CURRENT_SOURCE_DIR}/webgpu.idl)

add_custom_command(
    OUTPUT webgpu_glue.cpp webgpu_glue.js
    BYPRODUCTS parser.out WebIDLGrammar.pkl
    COMMAND ${PYTHON} ${WEBIDL_BINDER_SCRIPT} ${IDL_FILE} webgpu_glue
    DEPENDS ${IDL_FILE}
    COMMENT "Generating webgpu binding files"
    VERBATIM)

add_custom_target(webgpu-bindings ALL DEPENDS webgpu_glue.cpp)
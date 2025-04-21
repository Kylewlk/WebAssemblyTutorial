
cmake_minimum_required(VERSION 3.21)

# set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)
# set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(WebAssemblyTutorial)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

set(EMCC_COMMAND emcc)
if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(EMCC_COMMAND emcc.bat)
endif()

execute_process(
    COMMAND ${EMCC_COMMAND} --version
    RESULT_VARIABLE EMCC_RESULT
    OUTPUT_VARIABLE EMCC_VERSION_OUTPUT
    ERROR_VARIABLE EMCC_ERROR
    # ECHO_OUTPUT_VARIABLE
    OUTPUT_STRIP_TRAILING_WHITESPACE
    ERROR_STRIP_TRAILING_WHITESPACE
)

set(EMCC_VERSION "1.0.0")
if(EMCC_RESULT EQUAL 0)
    string(REGEX MATCH "emcc \\(Emscripten[^)]*\\) ([0-9]+\\.[0-9.]+)" _ "${EMCC_VERSION_OUTPUT}")
    message(STATUS "REGEX MATCH: ${_}")
    set(EMCC_VERSION ${CMAKE_MATCH_1})
    message(STATUS "Emscripten compiler (emcc) version: ${EMCC_VERSION}")
else()
    message(FATAL_ERROR "Failed to get emcc version. Error: ${EMCC_ERROR}")
endif()

add_subdirectory(00-hello)
add_subdirectory(module)

if (EMCC_VERSION VERSION_GREATER "4.0.0")
    add_subdirectory(webidl)
    add_subdirectory(webgpu)
    add_subdirectory(embind)
else()
    message(WARNING "Emscripten version is ${EMCC_VERSION} less than 4.0.0.")
endif()


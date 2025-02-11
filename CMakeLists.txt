
cmake_minimum_required(VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake)

project(WebAssemblyTutorial)

add_subdirectory(00-hello)

add_subdirectory(webidl)
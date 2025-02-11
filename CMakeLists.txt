
cmake_minimum_required(VERSION 3.21)

set(CMAKE_TOOLCHAIN_FILE $ENV{EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 17)

project(WebAssemblyTutorial)

add_subdirectory(00-hello)

add_subdirectory(webidl)

add_subdirectory(webgpu)
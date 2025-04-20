cmake_minimum_required(VERSION 3.21)
project(WebAssemblyTutorial-embind)

set(CMAKE_EXECUTABLE_SUFFIX .html)

add_executable(embind embind.cpp)

target_link_options(embind PRIVATE 
    "--shell-file=${CMAKE_CURRENT_SOURCE_DIR}/shell.html"
    "-lembind"
)

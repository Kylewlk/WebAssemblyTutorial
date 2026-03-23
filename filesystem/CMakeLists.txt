cmake_minimum_required(VERSION 3.21)
project(filesystem)
add_executable(filesystem filesystem.cpp)

target_link_options(filesystem PRIVATE
    --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/data/@/d
)


 if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
     set_target_properties(filesystem PROPERTIES SUFFIX ".html")
 endif()
 
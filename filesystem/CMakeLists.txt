cmake_minimum_required(VERSION 3.21)
project(filesystem)
add_executable(filesystem filesystem.cpp)

target_link_options(filesystem PRIVATE

    # 增加 SHELL: 前缀，Ninja 会直接报字符串当中命令
    "SHELL:--preload-file ${CMAKE_CURRENT_SOURCE_DIR}/data/@/d"

    "--post-js=${CMAKE_CURRENT_SOURCE_DIR}/post.js"
)


 if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
     set_target_properties(filesystem PROPERTIES SUFFIX ".html")
 endif()
 
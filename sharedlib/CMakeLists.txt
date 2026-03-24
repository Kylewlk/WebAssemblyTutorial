cmake_minimum_required(VERSION 3.21)
project(sharedlib)


add_library(liba SHARED liba.cpp)

# 必须设置 SIDE_MODULE 标志
target_link_options(liba PRIVATE "-sSIDE_MODULE=1")

# 建议后缀设为 .wasm 或 .so 以符合习惯
set_target_properties(liba PROPERTIES SUFFIX ".wasm")

set_target_properties(liba PROPERTIES POSITION_INDEPENDENT_CODE ON)

add_executable(sharedlib sharedlib.cpp)

target_link_options(sharedlib PRIVATE
    "-sMAIN_MODULE=1"
)

set_target_properties(sharedlib PROPERTIES POSITION_INDEPENDENT_CODE ON)

target_link_libraries(sharedlib PRIVATE liba)


 if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
     set_target_properties(sharedlib PROPERTIES SUFFIX ".html")
 endif()
 
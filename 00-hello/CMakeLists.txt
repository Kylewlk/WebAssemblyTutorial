cmake_minimum_required(VERSION 3.21)
project(hello)
add_executable(hello hello.cpp)
if (CMAKE_SYSTEM_NAME STREQUAL Emscripten)
    set(CMAKE_EXECUTABLE_SUFFIX .js)
endif()

if(EMCC_VERSION VERSION_GREATER "4.0.0")
    target_compile_options(hello PRIVATE
        "-sMEMORY64=1"
    )

    target_link_options(
        hello PRIVATE
        "-sMEMORY64=1"
    )
endif()

 message(STATUS "CMAKE_HOST_SYSTEM_NAME: ${CMAKE_HOST_SYSTEM_NAME}")
 message(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")
 
 if (CMAKE_SYSTEM_NAME STREQUAL "Emscripten")
     set_target_properties(hello PROPERTIES SUFFIX ".html")
 endif()
 
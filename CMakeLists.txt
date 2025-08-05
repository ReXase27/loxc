cmake_minimum_required(VERSION 4.0)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(loxc LANGUAGES C)

add_executable(loxc 
    src/main.c
    src/chunk.c
    src/compiler.c
    src/debug.c
    src/memory.c
    src/scanner.c
    src/value.c
    src/vm.c
)
target_include_directories(loxc PRIVATE 
    include
)
target_compile_options(loxc PRIVATE 
    -Wall
    -Wextra
    -Wpedantic
    -Werror
)
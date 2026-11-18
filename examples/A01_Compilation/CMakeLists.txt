cmake_minimum_required(VERSION 3.20)
project(A01_Compilation LANGUAGES C)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_library(math STATIC math.c)
target_compile_options(math PRIVATE -Wall -Wextra -Wcast-qual -Werror)
target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(program main.c)
target_link_libraries(program PRIVATE math)
target_compile_options(program PRIVATE -Wall -Wextra -Wcast-qual -Werror)

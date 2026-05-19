cmake_minimum_required(VERSION 3.10)
project(minigit C)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(include)

add_executable(
    main src/main.c
    src/cli/parser.c
)

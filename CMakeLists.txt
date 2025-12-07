cmake_minimum_required(VERSION 3.20)
project(SPMCQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(benchmark REQUIRED)

add_library(itch_parser src/parser.cpp)
target_include_directories(itch_parser PUBLIC include)

add_executable(parser_bench
    benchmark/main.cpp
    benchmark/bm_parser.cpp
    benchmark/bm_ops.cpp
)
target_compile_options(parser_bench PRIVATE -g -fno-omit-frame-pointer)
target_link_libraries(parser_bench PRIVATE itch_parser benchmark::benchmark)

add_executable(test
    src/main.cpp
)
target_compile_options(test PRIVATE -g -fno-omit-frame-pointer)
target_link_libraries(test PRIVATE itch_parser)

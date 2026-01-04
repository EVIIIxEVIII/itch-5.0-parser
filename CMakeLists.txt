cmake_minimum_required(VERSION 3.20)
project(SPMCQueue LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native -g -fno-omit-frame-pointer -fno-optimize-sibling-calls")

find_package(benchmark REQUIRED)
find_package(absl REQUIRED)

add_library(itch_parser INTERFACE)
target_include_directories(itch_parser INTERFACE include)
target_link_libraries(itch_parser INTERFACE absl::flat_hash_map)

add_executable(parser_bench
    benchmark/main.cpp
    src/utils.cpp
    benchmark/bm_parser_v1.cpp
    benchmark/bm_parser_v2.cpp
)

target_compile_options(parser_bench PRIVATE)
target_link_libraries(parser_bench PRIVATE itch_parser benchmark::benchmark)

add_executable(test
    src/main.cpp
)
target_include_directories(test PRIVATE
    /opt/intel/oneapi/vtune/latest/sdk/include
)
target_link_directories(test PRIVATE
    /opt/intel/oneapi/vtune/latest/sdk/lib64
)
target_link_libraries(test PRIVATE itch_parser ittnotify)

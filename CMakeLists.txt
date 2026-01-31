cmake_minimum_required(VERSION 3.20)
project(ITCH LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SANITIZE "Enable sanitizers" OFF)

find_package(absl REQUIRED)
find_package(benchmark REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(DPDK REQUIRED libdpdk)

add_library(itch_parser INTERFACE)
target_include_directories(itch_parser INTERFACE include)
target_link_libraries(itch_parser INTERFACE absl::flat_hash_map benchmark::benchmark pthread)

add_executable(benchmark src/main.cpp src/benchmarks/benchmark_utils.cpp)
target_link_libraries(benchmark PRIVATE itch_parser ${DPDK_LIBRARIES})

add_executable(perf_bench src/main.cpp src/benchmarks/benchmark_utils.cpp)
target_link_libraries(perf_bench PRIVATE itch_parser ${DPDK_LIBRARIES})
target_compile_definitions(perf_bench PRIVATE PERF)
target_compile_options(perf_bench PRIVATE -O3 -march=native -DNDEBUG)

if (SANITIZE)
    set(SAN_FLAGS
        -fsanitize=address,undefined
        -fno-omit-frame-pointer
        -O1
        -g
    )
    target_compile_options(benchmark PRIVATE ${SAN_FLAGS})
    target_link_options(benchmark PRIVATE ${SAN_FLAGS})
endif()


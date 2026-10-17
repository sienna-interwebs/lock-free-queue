cmake_minimum_required(VERSION 3.20)

project(lock_free_queue LANGUAGES CXX)

find_package(benchmark REQUIRED)

add_executable(main
    src/queues/atomic_spsc/queue.cpp
    src/main.cpp
)

add_executable(queue_bench
    src/benchmark/queue_bench.cpp
    src/queues/atomic_spsc/queue.cpp
)

set_property(TARGET main PROPERTY CXX_STANDARD 17)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET main PROPERTY CXX_EXTENSIONS OFF)

target_compile_features(queue_bench PRIVATE cxx_std_17)

target_compile_options(main PRIVATE -Wall -Wextra)
target_compile_options(queue_bench PRIVATE -Wall -Wextra)

target_include_directories(main PRIVATE src)
target_include_directories(queue_bench PRIVATE src)

target_link_libraries(queue_bench PRIVATE benchmark::benchmark)

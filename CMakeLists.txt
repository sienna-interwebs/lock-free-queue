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

add_executable(mpsc_race
    src/experiments/mpsc_race.cpp
    src/queues/mpsc_queue/queue.cpp
)

find_package(Threads REQUIRED)
target_link_libraries(mpsc_race PRIVATE Threads::Threads)

set_property(TARGET main PROPERTY CXX_STANDARD 17)
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET main PROPERTY CXX_EXTENSIONS OFF)

target_compile_features(queue_bench PRIVATE cxx_std_17)

target_compile_options(main PRIVATE -Wall -Wextra)
target_compile_options(queue_bench PRIVATE -Wall -Wextra)

target_include_directories(main PRIVATE src)
target_include_directories(queue_bench PRIVATE src)

target_link_libraries(queue_bench PRIVATE benchmark::benchmark)

target_include_directories(mpsc_race PRIVATE src)
target_compile_features(mpsc_race PRIVATE cxx_std_17)
target_compile_options(mpsc_race PRIVATE -Wall -Wextra)

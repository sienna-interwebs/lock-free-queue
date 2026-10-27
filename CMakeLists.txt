cmake_minimum_required(VERSION 3.20)
project(lock_free_queue LANGUAGES CXX)

find_package(benchmark REQUIRED)
find_package(Threads REQUIRED)


add_executable(main
    src/queues/atomic_spsc/queue.cpp
    src/main.cpp
)

add_executable(queue_bench
    src/benchmark/queue_bench.cpp
    src/queues/atomic_spsc/queue.cpp
    src/queues/mpsc_queue/queue.cpp
)

add_executable(mpsc_race
    src/experiments/mpsc_race.cpp
    src/queues/mpsc_queue/queue.cpp
)

add_executable(spsc_race
    src/experiments/spsc_race.cpp
    src/queues/atomic_spsc/queue.cpp
)

add_executable(mpsc_wrap
    src/experiments/mpsc_wrap.cpp
    src/queues/mpsc_queue/queue.cpp
)




set(ALL_TARGETS main queue_bench mpsc_race spsc_race mpsc_wrap)


foreach(target IN LISTS ALL_TARGETS)
    target_include_directories(${target} PRIVATE src)
    target_compile_features(${target} PRIVATE cxx_std_17)
    target_compile_options(${target} PRIVATE -Wall -Wextra)
endforeach()

# Target-specific properties for 'main'
set_property(TARGET main PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET main PROPERTY CXX_EXTENSIONS OFF)


target_link_libraries(queue_bench PRIVATE benchmark::benchmark)
target_link_libraries(mpsc_race   PRIVATE Threads::Threads)
target_link_libraries(spsc_race   PRIVATE Threads::Threads)
target_link_libraries(mpsc_wrap   PRIVATE Threads::Threads)

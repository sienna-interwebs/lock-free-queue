# ring-buffer

a bounded fifo ring buffer implemented in c++.

the project currently contains two spsc implementations:

* mutex-protected spsc
* atomic spsc using acquire/release and relaxed memory ordering

the queue uses circular indexing, configurable capacity, and `o(1)` enqueue/dequeue operations.

## concurrency

the atomic spsc implementation gives the producer exclusive ownership of `put_index` and the consumer exclusive ownership of `position`.

acquire/release synchronization publishes completed writes and reads between the producer and consumer without a mutex.

concurrent correctness was validated with 100 million successful enqueues and 100 million successful dequeues.

## benchmarking

a custom benchmarking and statistics framework collects repeated runtime measurements.

statistics include:

* minimum and maximum
* mean and median
* standard deviation
* p25, p75, p90, p95, p99
* iqr
* outlier count

the atomic spsc implementation was benchmarked across capacities from 16 to 262,144 elements, with 1 billion successful enqueues and 1 billion successful dequeues per trial.

benchmark results are analyzed with python and matplotlib.

## results

the current atomic spsc benchmark shows little change in mean runtime across the tested capacities.

the more noticeable differences appear in variability and tail behavior. capacity 4096 produced the lowest standard deviation and p95/p99 values in the current experiment, while capacity 16 showed the highest variability and p95/p99 values.

## project structure

```text
ring-buffer/
├── README.md
├── .gitignore
├── src/
│   ├── queues/
│   │   ├── mutex_spsc/
│   │   │   ├── queue.hpp
│   │   │   └── queue.cpp
│   │   └── atomic_spsc/
│   │       ├── queue.hpp
│   │       └── queue.cpp
│   ├── benchmark/
│   │   ├── benchmark.hpp
│   │   └── benchmark.cpp
│   ├── stats/
│   │   ├── stats.hpp
│   │   └── stats.cpp
│   └── main.cpp
├── analysis/
│   └── pyplots.py
└── results/
    ├── mutex_spsc_benchmark_stats.csv
    ├── atomic_spsc_benchmark_stats.csv
    └── plots/
```

## next

extend the queue toward mpsc and mpmc designs; benchmark synchronization and memory-layout changes against the existing spsc implementations.

# concurrent ring buffer

a bounded fifo ring buffer implemented in c++ with mutex-based and atomic single-producer, single-consumer (spsc) queues. the project focuses on concurrency correctness, synchronization costs, and repeatable performance measurement.

## features

* **bounded fifo queue:** configurable capacity, circular indexing, explicit full and empty handling, and `o(1)` enqueue/dequeue operations.
* **mutex-based spsc:** synchronized queue implementation using `std::mutex`.
* **atomic spsc:** producer/consumer coordination using atomic counters and acquire/release memory ordering, without a mutex in the queue operations.
* **concurrent correctness tests:** producer/consumer tests for fifo ordering and successful operation counts.
* **benchmarking framework:** repeated trials with runtime statistics including mean, median, standard deviation, percentiles, interquartile range (iqr), and outlier detection.
* **performance experiments:** comparisons of queue capacities, cache-line alignment, and power-of-two indexing.

## benchmark methodology

the benchmark runs a producer and consumer concurrently and measures the total time required to complete a fixed number of successful queue operations.

the atomic spsc benchmark uses workloads of **1 billion successful enqueues and 1 billion successful dequeues per trial**. the benchmark records runtime distributions across queue capacities rather than relying on a single run or average.

reported statistics include:

* mean and median runtime
* standard deviation and interquartile range
* p90, p95, and p99 runtime percentiles
* minimum and maximum runtime
* detected outliers

these percentiles describe **whole-workload runtime**, not the latency of an individual enqueue or dequeue.

## performance investigations

### queue capacity

the initial atomic spsc benchmark showed mean runtimes of approximately **11.5–11.8 seconds** across tested capacities from **16 to 262,144 elements**. within that experiment, changing capacity had relatively little effect on mean runtime.

### cache-line alignment

aligning the producer and consumer counters to **128-byte boundaries** was associated with approximately **14–19% lower mean runtime** at comparable tested capacities in separate benchmark runs. this result motivates further investigation into cache-line contention and measurement variability; it does not, by itself, prove that false sharing was the sole cause.

### power-of-two indexing

power-of-two capacities allow circular indexing with a bitmask instead of modulo. benchmark results varied by capacity, so the change is being evaluated as a workload-dependent optimization rather than assumed to be universally faster.

## repository structure

```text
ring-buffer/
├── README.md
├── .gitignore
├── src/
│   ├── queues/
│   │   ├── mutex_spsc/
│   │   └── atomic_spsc/
│   ├── benchmark/
│   ├── stats/
│   └── main.cpp
├── analysis/
│   └── pyplots.py
└── results/
```

## next steps

* extend the queue designs toward multi-producer, single-consumer (mpsc) operation.
* add correctness tests tailored to multi-producer synchronization.
* compare throughput and runtime distributions across queue implementations under equivalent workloads.
* continue toward multi-producer, multi-consumer (mpmc) designs after validating the mpsc implementation.

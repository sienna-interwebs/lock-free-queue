# ring-buffer

a bounded fifo ring buffer in c++; currently implemented as a mutex-protected spsc queue.

the repo includes the queue implementation, a custom benchmark and statistics system, and python-based benchmark analysis :)

## current implementation

the queue provides:

* configurable capacity
* bounded fifo semantics
* circular indexing
* separate read and write indices
* explicit full and empty handling
* `put()` and `take()` operations
* `std::vector` storage
* `std::mutex` synchronization

the queue maintains:

* `position` ; the next element to remove
* `put_index` ; the next position to insert into
* `count` ; the number of elements currently in the queue
* `collection` ; the underlying storage

elements are not physically removed from the vector; the read and write indices advance through the storage using wraparound indexing.

when the queue is full, `put()` returns `false`.

when the queue is empty, `take()` returns `false`.

## concurrency

the current queue uses one producer and one consumer.

queue state is protected by a mutex; the producer and consumer can operate on the same queue without data races.

the mutex-protected implementation serves as the baseline for the lock-free queue implementations that follow.

## benchmarking

the repository includes a custom benchmarking and statistics system.

the current benchmark uses:

* 6 queue capacities
* 50 trials per capacity
* 10 million successful enqueues per trial
* 10 million successful dequeues per trial
* one producer thread
* one consumer thread

this produces 300 benchmark trials and 6 billion successful queue operations across the complete experiment.

the statistics system records:

* minimum
* maximum
* mean
* median
* standard deviation
* p25
* p75
* p90
* p95
* p99
* interquartile range
* outlier count

worker threads wait on a start signal before the timed workload begins; thread creation is excluded from the measured interval.

## benchmark analysis

benchmark results are exported to csv and analyzed with python.

the analysis currently produces three visualizations.

### average runtime

![average runtime]`capacity_ms_mutex_spsc.png`

mean runtime as a function of queue capacity; capacity is shown on a log2 axis.

### percentile distributions

![percentile distributions]`capacity_percentile_distribution_mutex_spsc.png`

runtime across several percentiles for each queue capacity; this shows the runtime distribution and upper tail.

### performance surface

![performance surface]`mutex_spsc_surface.png`

a 3d visualization of percentile, capacity, and runtime.

capacity is represented on a log2 scale; the surface between measured points is interpolated and does not represent directly benchmarked configurations.

the results describe this queue under this workload and environment; they are not a general performance model ! 

## project structure

```text
ring-buffer/
├── README.md
├── .gitignore
│
├── src/
│   ├── queue/
│   │   ├── my_queue.hpp
│   │   └── my_queue.cpp
│   │
│   ├── benchmark/
│   │   ├── benchmark.hpp
│   │   └── benchmark.cpp
│   │
│   ├── stats/
│   │   ├── stats.hpp
│   │   └── stats.cpp
│   │
│   └── main.cpp
│
├── analysis/
│   └── pyplots.py
│
└── results/
    ├── benchmark_stats.csv
    └── plots/
        ├── capacity_ms_mutex_spsc.png
        ├── capacity_percentile_distribution_mutex_spsc.png
        └── mutex_spsc_surface.png
```

source code, analysis code, and benchmark results are kept separate for ease of access.

## next

the queue is moving from mutex synchronization toward lock-free concurrency.

```text
mutex-protected spsc
        |
        v
lock-free spsc
        |
        v
mpsc
        |
        v
mpmc
        |
        v
comparative benchmarking
```

planned work:

* atomic synchronization
* c++ memory ordering
* lock-free spsc
* cache and cache-coherence effects
* mpsc
* mpmc
* comparative benchmarks against established queue implementations
* cpu and system-level performance analysis

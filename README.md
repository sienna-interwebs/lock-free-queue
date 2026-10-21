# lock-free-queue

bounded fifo ring buffers in c++, built up one concurrency guarantee at a time: a mutex-based spsc queue, an atomic spsc queue, and an atomic mpsc queue. the point of the project is less "here's a fast queue" and more "here's how i know what each queue does and doesn't guarantee"; every claim below comes with the experiment behind it and its limits.

## the queues

all three are bounded ring buffers with explicit full/empty handling: `put()` returns `false` when the queue is full, `take()` returns `false` when it's empty.

| queue | path | producers / consumers | how it coordinates |
|---|---|---|---|
| mutex spsc | `src/queues/mutex_spsc/` | 1 / 1 | a `std::mutex` around every operation; modulo indexing with a count |
| atomic spsc | `src/queues/atomic_spsc/` | 1 / 1 | two atomic indices with acquire/release ordering; no locks |
| atomic mpsc | `src/queues/mpsc_queue/` | many / 1 | producers claim slots with compare-and-swap; each slot has its own `ready` flag |

the atomic queues take a power-of-two exponent, so `Queue q(10)` has capacity 1024 and indexes with a bitmask instead of modulo. both align their two indices to 128 bytes (the cache line size on apple silicon) so the producer's and consumer's counters don't share a cache line.

### why the mpsc queue looks the way it does

the atomic spsc queue is correct for one producer, but it breaks with two. the mpsc queue fixes two separate problems, found one at a time:

1. **claiming.** two producers can read the same tail and both believe they own that slot. each atomic load and store is atomic on its own, but "load, compute, store" isn't. fix: claim the slot with `compare_exchange`, which checks and moves the tail as one indivisible step.
2. **publishing.** after the cas, the tail says "slot claimed" before the value is actually written, so a consumer reading the tail could take a slot that still holds junk. fix: split "claimed" from "ready". `put_index` now only coordinates producers; each slot gets an `std::atomic<bool> ready` that the producer sets with `memory_order_release` after writing the value, and the consumer reads with `memory_order_acquire`. if the consumer sees `ready == true`, it's guaranteed to see the value.

the single consumer side stays as cheap as spsc: there's only one thread calling `take()`, so `position` needs no cas.

## experiments

### race experiments: `spsc_race` and `mpsc_race`

two producers each put 1,000,000 distinct values (1 to 1,000,000 and 1,000,001 to 2,000,000) while a consumer thread takes all 2,000,000. the queue has capacity 2²¹, so `put()` never fails from being full. every value taken is tallied in a `seen` vector indexed by value, which counts three things:

- **lost:** values that never came out
- **duplicated:** values that came out more than once
- **phantom zeros:** zeros taken from slots nobody wrote (no producer ever puts 0)

counting values instead of items matters. the first version of this experiment only counted how many items came out, and it reported a perfect 2,000,000 against a queue that was losing values; a count can't tell a real value from a zero nobody wrote.

| run | lost | duplicated | phantoms | what it shows |
|---|---|---|---|---|
| atomic spsc, 2 producers, drained after | 16,795 | 0 | 16,795 | lost updates from producers colliding on a slot |
| mpsc with cas only, consumer running concurrently | 162 to 541 (varies) | 0 | equal to lost | claiming works; publishing doesn't |
| mpsc with cas + per-slot ready flags | 0 | 0 | 0 | consistent across ~10 runs |

in every buggy run, `lost` equals `phantoms`: each failure loses one real value and leaves one unwritten slot behind. that's the relationship the hand trace of the bug predicts.

**limits:** a clean run is evidence, not proof. the experiment only catches bugs that happen during that run, and because capacity is 2²¹, the ring never wraps around, so nothing here tests slot reuse.

### threadsanitizer

both race experiments were built with clang's threadsanitizer (`-fsanitize=thread`):

- **`spsc_race` (positive control):** tsan reports a data race between two producer threads writing the same `int` slot in `Queue::put`. this confirms the tool is wired up and can catch this bug in this setup.
- **`mpsc_race`:** no races reported.

tsan finds data races on non-atomic memory, not logic bugs. a queue that's wrong but does everything through atomics would still pass, which is why the `seen`-tally experiment and tsan check different things.

### benchmarks

**google benchmark round trip (`queue_bench`).** a single thread calls `put()` then `take()` on the atomic spsc queue. this measures the uncontended cost of one enqueue plus one dequeue, not concurrent throughput. on a macbook air m1:

- checked (fails the benchmark if `put` or `take` ever returns `false`): ~38.4 ns
- unchecked: ~39.0 ns

within-run noise was tiny (cv under 0.3%), and the gap survived random interleaving of repetitions, but its cause is unknown (likely code generation or layout, not the checks themselves). the conclusion is only that the checks have no measurable cost here, not that checking is faster.

**producer/consumer runtime study (`src/benchmark/benchmark.cpp`, `src/stats/`).** one producer and one consumer run concurrently, released together by a `std::promise` start signal, and the total time to complete a fixed number of successful operations is recorded over repeated trials. `Stats` reports mean, median, standard deviation, p25/p75/p90/p95/p99, iqr, and iqr-based outlier counts. these percentiles describe whole-workload runtime, not per-operation latency. results are in `results/*.csv`, plots are in `results/plots/`, and `analysis/pyplots.py` generates them.

earlier findings from this study:

- **capacity:** atomic spsc mean runtimes stayed in a narrow band (about 11.5 to 11.8 s) across capacities from 16 to 262,144; capacity had little effect.
- **cache-line alignment:** aligning the two counters to 128 bytes was associated with about 14 to 19% lower mean runtime in separate runs. that's consistent with reduced false sharing, but it doesn't prove false sharing was the only cause.
- **power-of-two indexing:** results varied by capacity, so the bitmask is treated as a workload-dependent optimization, not a guaranteed win.

**caveat:** the csvs don't record which workload size or build settings produced them, so the mutex and atomic results shouldn't be compared directly yet.

## build and run

requires cmake 3.20+, a c++17 compiler, and google benchmark (`brew install google-benchmark` on macos).

```sh
cmake -S . -B build
cmake --build build

./build/queue_bench --benchmark_repetitions=10
./build/spsc_race
./build/mpsc_race
```

threadsanitizer build, in its own build folder so the normal build stays untouched:

```sh
cmake -S . -B build_tsan -DCMAKE_CXX_FLAGS="-fsanitize=thread -g -O1"
cmake --build build_tsan --target spsc_race mpsc_race
MallocNanoZone=0 ./build_tsan/spsc_race
MallocNanoZone=0 ./build_tsan/mpsc_race
```

(`MallocNanoZone=0` just silences a harmless macos allocator warning under tsan.)

note: `src/benchmark/benchmark.cpp`, `src/tests/queue_test.cpp`, and the mutex spsc queue aren't wired into `CMakeLists.txt` right now.

## repository structure

```text
lock_free_queue/
├── CMakeLists.txt
├── src/
│   ├── queues/
│   │   ├── mutex_spsc/
│   │   ├── atomic_spsc/
│   │   └── mpsc_queue/
│   ├── benchmark/      # google benchmark round trip + producer/consumer runtime study
│   ├── experiments/    # spsc_race, mpsc_race
│   ├── stats/          # trial statistics
│   ├── tests/
│   └── main.cpp
├── analysis/
│   └── pyplots.py
└── results/
    └── plots/
```

## next

- build and profile on linux
- test the mpsc queue with a small capacity so the ring wraps and slots get reused
- turn the queue into a cmake library target instead of compiling `queue.cpp` into every executable
- add sanitizer builds and tests to ci
- multi-producer, multi-consumer (mpmc)

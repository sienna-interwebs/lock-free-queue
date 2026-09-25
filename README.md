# ring buffer

a fixed-capacity, bounded fifo queue implemented in c++.

## overview

this project implements a ring buffer using a fixed-size array and explicit read/write indices.

the queue currently supports:

* fifo insertion and removal
* fixed capacity
* detection of full and empty states
* wraparound of the underlying storage
* basic behavioral tests

## implementation

the queue maintains:

* a fixed-size array for storage
* an index for the next element to remove
* an index for the next element to insert
* a count of elements currently stored

when an index reaches the end of the underlying array, it wraps back to the beginning.

## build

compile with:

```bash
g++ src/main.cpp -std=c++17 -Wall -Wextra -o main
```

then run:

```bash
./main
```

## status

the current implementation is single-threaded. concurrency and lock-free synchronization are not yet implemented.

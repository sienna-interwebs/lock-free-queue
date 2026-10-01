# ring-buffer

a bounded FIFO queue implemented in C++.

## current implementation features

* fixed-capacity storage using `std::vector`
* separate read and write indices
* FIFO insertion and removal
* wraparound indexing
* full and empty detection
* configurable queue capacity
* `put()` and `take()` operations return success/failure
* thread-safe access using `std::mutex`
* concurrent producer and consumer test
* FIFO ordering verified under concurrent access

## design

the queue maintains:

* `position`; the next element to remove
* `put_index`; the next position to insert into
* `count`; the number of elements currently in the queue
* `collection`; the underlying storage

the queue does not physically remove elements from the vector. instead, `position` and `put_index` move around the storage using wraparound indexing.

when the queue is full, `put()` returns `false`.

when the queue is empty, `take()` returns `false`.

all queue state is protected by a mutex, so a producer and consumer can safely operate on the same queue from different threads.

## testing

the current test creates one producer thread and one consumer thread.

the producer inserts five values:

`0, 100, 200, 300, 400`

the consumer removes five values and verifies that they arrive in FIFO order.

failed `put()` and `take()` attempts are retried until the required number of successful operations has completed.

## next steps

* benchmark the mutex-protected queue
* learn and apply atomic operations
* design an SPSC queue without a mutex
* benchmark the SPSC implementation against the mutex-protected version
* investigate memory ordering and cache effects

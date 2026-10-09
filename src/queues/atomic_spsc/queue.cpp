#include "queues/atomic_spsc/queue.hpp"
#include<iostream>
#include<vector>
#include<atomic>


Queue::Queue(int p2exp) : collection(1LL << p2exp) {

        this->position.store(0, std::memory_order_relaxed);
        this->put_index.store(0, std::memory_order_relaxed);
        this->mask = (1LL << p2exp) - 1;

}

bool Queue::take(int& value) {

        int snapshot = this->position.load(std::memory_order_relaxed);
        if (this->put_index.load(std::memory_order_acquire) - snapshot == 0) {
            return false;
        }
        value = this->collection[snapshot & this->mask];
        this->position.fetch_add(1, std::memory_order_release);

        return true;
}

bool Queue::put(int value) {

        int snapshot = this->put_index.load(std::memory_order_relaxed);
        if (snapshot - this->position.load(std::memory_order_acquire) >= static_cast<int>(this->collection.size())) {
            return false;
        }
        this->collection[snapshot & this->mask] = value;
        this->put_index.fetch_add(1, std::memory_order_release);

        return true;
}

#include "queues/mpsc_queue/queue.hpp"
#include<iostream>
#include<vector>
#include<atomic>


MpscQueue::MpscQueue(int p2exp) : collection(1LL << p2exp) {

        this->position.store(0, std::memory_order_relaxed);
        this->put_index.store(0, std::memory_order_relaxed);
        this->mask = (1LL << p2exp) - 1;

}

bool MpscQueue::take(int& value) {

        size_t snapshot = this->position.load(std::memory_order_relaxed);
        if (this->collection[snapshot & this->mask].ready.load(std::memory_order_acquire) == false) {
            return false;
        }
        value = this->collection[snapshot & this->mask].value;
        this->collection[snapshot & this->mask].ready = true;
        this->position.fetch_add(1, std::memory_order_release);

        return true;
}

bool MpscQueue::put(int value) {

        size_t snapshot = this->put_index.load(std::memory_order_relaxed);

        while (true) {


            if (snapshot - this->position.load(std::memory_order_acquire) >= static_cast<size_t>(this->collection.size())) {
                return false;
            }

            if (this->put_index.compare_exchange_strong(snapshot, snapshot+1)) {
                this->collection[snapshot & this->mask].value = value;
                this->collection[snapshot & this->mask].ready.store(true, std::memory_order_release);
                break;
            }
        }

        return true;
}

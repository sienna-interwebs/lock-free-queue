#ifndef ATOMIC_SPSC_QUEUE_HPP
#define ATOMIC_SPSC_QUEUE_HPP

#include<iostream>
#include<vector>
#include<atomic>

class Queue {
    private:

        std::vector<int> collection;
        alignas(128) std::atomic<size_t> position;
        alignas(128) std::atomic<size_t> put_index;
        int mask;

    public:

        Queue(int p2exp);

        bool take(int& value);

        bool put(int value);

};

#endif

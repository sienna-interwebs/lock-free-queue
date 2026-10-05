#ifndef ATOMIC_SPSC_QUEUE_HPP
#define ATOMIC_SPSC_QUEUE_HPP

#include<iostream>
#include<vector>
#include<atomic>

class Queue {
    private:

        std::vector<int> collection;
        std::atomic<int> position;
        std::atomic<int> put_index;

    public:

        Queue(int size);

        bool take(int& value);

        bool put(int value);

};

#endif

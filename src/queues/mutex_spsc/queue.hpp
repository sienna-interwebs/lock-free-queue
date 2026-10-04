#ifndef MUTEX_SPSC_QUEUE_HPP
#define MUTEX_SPSC_QUEUE_HPP

#include<iostream>
#include<vector>
#include<mutex>

class Queue {
    private:

        std::mutex m;
        std::vector<int> collection;
        int position;
        int put_index;
        int count;

    public:

        Queue(int size);

        bool take(int& value);

        bool put(int value);

        int len();

};

#endif

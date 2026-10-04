#include "queue.hpp"
#include<iostream>
#include<vector>
#include<mutex>


Queue::Queue(int size) : collection(size) {
        this->position = 0;
        this->put_index = 0;
        this->count = 0;
}

bool Queue::take(int& value) {
        m.lock();

        if (this->count == 0) {
            m.unlock();
            return false;
        }
        value = this->collection[this->position];
        this->position = (this->position + 1) % collection.size();
        this->count--;

        m.unlock();

        return true;
}

bool Queue::put(int value) {
        m.lock();

        if (this->count == static_cast<int>(collection.size())) {
            m.unlock();
            return false;
        }
        this->collection[this->put_index] = value;
        this->put_index = (this->put_index + 1) % collection.size();
        this->count++;

        m.unlock();

        return true;
}

int Queue::len() {
    int temp;
    m.lock();
    temp = count;
    m.unlock();
    return temp;
}

#include "queues/atomic_spsc/queue.hpp"
#include<iostream>

int main() {

    Queue q(1);

    bool result = q.put(34);

    if (result) {
        std::cout << "insertion works";
    }
    else {
        std::cout << "insertion doesn't work";
    }

    q.put(74);
    int value;
    result = q.take(value);

    if (value == 34) {

        std::cout << "expected, passed";

    }
    else {
        std::cout << "failed";
    }
}

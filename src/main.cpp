#include "queues/atomic_spsc/queue.hpp"
#include<iostream>

int main() {

    Queue q(5);

    if (q.put(42)) {
        std::cout << "test passed, 42 added :) \n";
    } else {
        std::cout << "test failed, 42 not added :( \n";
    }

    int retrieved_value{};
    if (q.take(retrieved_value)) {
        std::cout << "test passed, value retrieved :) \n";

        if (retrieved_value == 42) {
            std::cout << "test passed, value matches! \n";
        } else {
            std::cout << "test failed, wrong value :( \n";
        }
    } else {
        std::cout << "test failed, value not pulled out :( \n";
    }

    int empty_check{};
    if (q.take(empty_check)) {
        std::cout << "test failed :( took a value out while empty \n";
    } else {
        std::cout << "test passed :) didn't take a value out while empty \n";
    }

    return 0;
}

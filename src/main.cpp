#include "queues/atomic_spsc/queue.hpp"
#include<iostream>
#include<thread>

void producing(Queue& q) {
    int successful = 0;
    while (successful < 100000000) {
        bool result = q.put(successful * 100);
        if (result) {
            successful++;
        }
    }
}

void consuming(Queue& q) {
    int value;
    int successful = 0;
    while (successful < 100000000) {
        bool result = q.take(value);
        if (result) {
            if (value != successful * 100) {
                std::cout << "failed :(\n";
            }
            successful++;
        }
    }
}

int main() {

    Queue q(16384);

    std::thread producer(producing, std::ref(q));
    std::thread consumer(consuming, std::ref(q));

    producer.join();
    consumer.join();

    return 0;


}

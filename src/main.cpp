#include "my_queue.hpp"
#include<iostream>
#include<vector>
#include<mutex>
#include<thread>
#include<chrono>
#include<future>


void producing(Queue& q, std::shared_future<void> start_signal) {
    start_signal.wait();

    int successful = 0;
    while (successful < 10000000) {
        bool result = q.put(successful * 10);
        if (result) {
            successful++;
        }
    }
}

void consuming(Queue& q, std::shared_future<void> start_signal) {
    start_signal.wait();

    int value;
    int successful = 0;
    while (successful < 10000000) {
        bool result = q.take(value);
        if (result) {
            successful++;
        }
    }
}

void run_benchmark(int capacity) {
    Queue q(capacity);

    std::promise<void> my_promise;
    auto go_future = my_promise.get_future().share();

    std::thread producer(producing, std::ref(q), go_future);
    std::thread consumer(consuming, std::ref(q), go_future);

    auto ti = std::chrono::steady_clock::now();

    my_promise.set_value();

    producer.join();
    consumer.join();

    auto tf = std::chrono::steady_clock::now();

    std::chrono::duration<double, std::milli> delta_t = tf - ti;

    std::cout << capacity << " elements: " << delta_t.count() << "ms \n";

}

int main() {

    run_benchmark(16);
    run_benchmark(64);
    run_benchmark(256);
    run_benchmark(1024);
    run_benchmark(4096);
    run_benchmark(16384);



}

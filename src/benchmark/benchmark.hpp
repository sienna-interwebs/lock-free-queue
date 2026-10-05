#ifndef BENCHMARK_HPP
#define BENCHMARK_HPP

#include "queues/atomic_spsc/queue.hpp"
#include "stats/stats.hpp"
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <future>
#include <map>

void producing(Queue& q, std::shared_future<void> start_signal);
void consuming(Queue& q, std::shared_future<void> start_signal);
void run_benchmark(int capacity, Stats& collector);

#endif

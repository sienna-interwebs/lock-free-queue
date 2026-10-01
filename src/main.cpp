#include "benchmark.hpp"
#include "my_queue.hpp"
#include "stats.hpp"
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>
#include <chrono>
#include <future>
#include <map>

int main() {
    std::map<int, Stats> benchmark_stats;
    std::vector<int> capacities = {16, 64, 256, 1024, 4096, 16384};

    for (int i{}; i < static_cast<int>(capacities.size()); i++){
        std:: cout << "benchmark run for " << capacities[i] << " elements: \n";
        for (int j{}; j < 50 ; j++) {
            run_benchmark(capacities[i], benchmark_stats[capacities[i]]);
        }
        benchmark_stats[capacities[i]].print(capacities[i]);
    }

}

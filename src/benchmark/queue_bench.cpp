#include <benchmark/benchmark.h>
#include "queues/atomic_spsc/queue.hpp"

static void round_trip_checked_benchmark(benchmark::State& state) {
    Queue q(10);

    int value = 13;


    for (auto _ : state) {


        bool result_insertion = q.put(value);
        bool result_deletion = q.take(value);

        if (result_insertion) {


            benchmark::DoNotOptimize(result_insertion);

        }

        else {
            state.SkipWithError("put failed, queue full");
            break;
        }
        if (result_deletion) {
            benchmark::DoNotOptimize(result_deletion);
        }
        else {
            state.SkipWithError("take failed, queue empty");
            break;
        }
    }
}

static void round_trip_unchecked_benchmark(benchmark::State& state) {

    Queue q(10);

    int value = 13;

    for (auto _ : state) {


        bool result_insertion = q.put(value);
        bool result_deletion = q.take(value);

        benchmark::DoNotOptimize(result_insertion);
        benchmark::DoNotOptimize(result_deletion);

    }

}

BENCHMARK(round_trip_checked_benchmark);
BENCHMARK(round_trip_unchecked_benchmark);
BENCHMARK_MAIN();

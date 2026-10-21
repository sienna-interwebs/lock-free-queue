#include "queues/atomic_spsc/queue.hpp"
#include <iostream>
#include<thread>
#include<vector>

void producing(Queue& q, int start) {

    int successful = 0;
    while (successful < 1000000) {
        bool result = q.put(start);
        if (result) {
            successful++;
            start++;
        }
    }
}

void consuming(Queue& q, std::vector<int>& seen) {

    int value;
    int successful = 0;
    while (successful < 2000000) {
        bool result = q.take(value);
        if (result) {
            seen[value]++;
            successful++;

        }
    }
}


int main() {

    std::vector<int> seen(2000001, 0);


    int value = 0;
    int counter = 0;

    int lost = 0;
    int remained = 0;
    int duplicated = 0;

    Queue q(21);

    std::thread producer1(producing, std::ref(q), 1);
    std::thread producer2(producing, std::ref(q), 1000001);
    std::thread consumer1(consuming, std::ref(q), std::ref(seen));

    producer1.join();
    producer2.join();
    consumer1.join();

    while (q.take(value)){

        counter++;
        seen[value]++;

    }

    std::cout << counter << "\n";

    for (int i{1}; i < 2000001; i++) {

        if (seen[i] == 0) {
            lost++;
        }
        else {
            remained++;
        }
        if (seen[i] > 1) {
            duplicated++;
        }

    }

    std::cout << "items available: " << remained << "\n";
    std::cout << "items lost: " << lost << "\n";
    std::cout << "items duplicated: " << duplicated << "\n";
    std::cout << seen[0] << "\n";

}

#include "queues/mpsc_queue/queue.hpp"
#include <iostream>
#include<thread>
#include<vector>

void producing(MpscQueue& q, int start) {

    int successful = 0;
    while (successful < 1000000) {
        bool result = q.put(start);
        if (result) {
            successful++;
            start++;
        }
    }
}


int main() {

    MpscQueue q(21);

    std::thread producer1(producing, std::ref(q), 1);
    std::thread producer2(producing, std::ref(q), 1000001);

    producer1.join();
    producer2.join();

    std::vector<int> seen(2000001, 0);


    int value = 0;
    int counter = 0;

    int lost = 0;
    int remained = 0;
    int duplicated = 0;

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

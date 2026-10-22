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

void consuming(MpscQueue& q, std::vector<int>& seen) {

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

    MpscQueue q(21);

    std::thread producer1(producing, std::ref(q), 1);
    std::thread producer2(producing, std::ref(q), 1);
    std::thread producer3(producing, std::ref(q), 1);
    std::thread producer4(producing, std::ref(q), 1);
    std::thread producer5(producing, std::ref(q), 1);
    std::thread producer6(producing, std::ref(q), 1);
    std::thread producer7(producing, std::ref(q), 1);
    std::thread producer8(producing, std::ref(q), 1);

    std::thread consumer1(consuming, std::ref(q), std::ref(seen));

    producer1.join();
    producer2.join();
    producer3.join();
    producer4.join();
    producer5.join();
    producer6.join();
    producer7.join();
    producer8.join();
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

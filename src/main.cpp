#include<iostream>
#include<vector>
#include<mutex>
#include<thread>

class Queue {
    private:

        std::mutex m;
        std::vector<int> collection;
        int position;
        int put_index;
        int count;

    public:
        Queue(int size) : collection(size) {
            this->position = 0;
            this->put_index = 0;
            this->count = 0;
        }

        bool take(int& value) {
            m.lock();

            if (this->count == 0) {
                m.unlock();
                return false;
            }
            value = this->collection[this->position];
            this->position = (this->position + 1) % collection.size();
            this->count--;

            m.unlock();

            return true;
        }

        bool put(int value) {
            m.lock();

            if (this->count == collection.size()) {
                m.unlock();
                return false;
            }
            this->collection[this->put_index] = value;
            this->put_index = (this->put_index + 1) % collection.size();
            this->count++;

            m.unlock();

            return true;
        }

        int len() {
            int temp;
            m.lock();
            temp = count;
            m.unlock();
            return temp;
        }
};

void producing(Queue& q) {
    int successful = 0;
    while (successful < 5) {
        bool result = q.put(successful * 100);
        if (result) {
            successful++;
        }
    }
}

void consuming(Queue& q) {
    int value;
    int successful = 0;
    while (successful < 5) {
        bool result = q.take(value);
        if (result) {
            if (value == successful * 100) {
                std::cout << "passed :) \n";
            }
            else {
                std::cout << "failed :( \n";
            }
            successful++;
        }
    }
}

int main() {

    Queue p(5);

    std::thread producer(producing, std::ref(p));
    std::thread consumer(consuming, std::ref(p));

    producer.join();
    consumer.join();

    return 0;

}

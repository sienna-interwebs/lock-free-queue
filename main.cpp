
#include<iostream>
class Queue {
    private:

    public:

        int collection[5];
        int position;
        int put_index;
        int count;

        Queue() {
            this->position = 0;
            this->put_index = 0;
            this->count = 0;
        }

        bool take(int& value) {
            if (this->count == 0) {
                return false;
            }
            value = this->collection[this->position];
            this->position = (this->position + 1) % 5;
            this->count--;
            return true;
        }

        bool put(int value) {
            if (this->count == 5) {
                return false;
            }
            this->collection[this->put_index] = value;
            this->put_index = (this->put_index + 1) % 5;
            this->count++;
            return true;
        }

};

void print_all(Queue& q) {
    for (int i{}; i < 5; i++) {
        std::cout << q.collection[i] << "\n";
    }
}

void test_fifo(Queue& q) {
    int value = 100;
    for (int i = 0; i < 5; i++) {
        q.put(value);
        value *= 2;
    }
    print_all(q);

    for (int i{}; i < 5; i++) {
        q.take(value);
        value /= 2;
    }
    print_all(q);
}

void test_empty_queue(Queue& q) {
    int value;
    bool result = q.take(value);
    if (result) {
        std::cout << "test failed: empty queue has been taken";
    } else {
        std::cout << "test passed: empty queue has not been taken";
    }
}

void test_full_queue(Queue& q) {
    int value = 100;
    bool result = q.put(value);
    if (result) {
        std::cout << "test failed: full queue has been filled";
    } else {
        std::cout << "test passed: full queue has not been filled";
    }
}

void arbitrary_fill(Queue& q) {
    for (int i{}; i < 5; i++) {
        int value = i * 10;
        q.put(value);
    }
}

void test_ring(Queue& q) {
    int value;
    q.put(100);
    if (q.put_index == 1) {
        std::cout << "test passed: empty ring has been filled with 1 element" << "\n";
    } else {
        std::cout << "test failed: empty ring has not been filled with 1 element" << "\n";
    }
    q.put(200);
    if (q.put_index == 2) {
        std::cout << "test passed: empty ring has been filled with 2 elements" << "\n";
    } else {
        std::cout << "test failed: empty ring has not been filled with 2 elements" << "\n";
    }
    q.put(300);
    if (q.put_index == 3) {
        std::cout << "test passed: empty ring has been filled with 3 elements" << "\n";
    } else {
        std::cout << "test failed: empty ring has not been filled with 3 elements" << "\n";
    }
    q.take(value);
    if (q.position == 1 && value == 100) {
        std::cout << "test passed: element taken from empty ring" << "\n";
    } else {
        std::cout << "test failed: element taken from empty ring" << "\n";
    }

    q.put(500);
    if (q.put_index == 4 && q.count == 3) {
        std::cout << "test passed: ring has been filled with 3 elements" << "\n";
    } else {
        std::cout << "test failed: either count or put_index is incorrect" << "\n";
    }
    q.take(value);
    if (q.count == 2 && q.position == 2 && value == 200) {
        std::cout << "test passed: element taken from ring" << "\n";
    } else {
        std::cout << "test failed: element taken from ring" << "\n";
    }
    q.put(700);
    if (q.put_index == 0 && q.count == 3) {
        std::cout << "test passed: ring has been filled with 3 elements" << "\n";
    } else {
        std::cout << "test failed: either count or put_index is incorrect" << "\n";
    }
}

int main() {
    Queue q;
    Queue p;
    Queue r;
    test_fifo(q);
    test_empty_queue(p);
    test_ring(p);
    arbitrary_fill(r);
    test_full_queue(r);
}

#include<iostream>
class Queue {
    private:

        int collection[5];
        int position;
        int put_index;
        int count;

    public:
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

void test_empty_queue(Queue& q) {
    int value;
    bool result = q.take(value);
    if (result) {
        std::cout << "test failed: empty queue has been taken" << "\n";
    } else {
        std::cout << "test passed: empty queue has not been taken" << "\n";
    }
}

void test_full_queue(Queue& q) {
    int value = 100;
    bool result = q.put(value);
    if (result) {
        std::cout << "test failed: full queue has been filled" << "\n";
    } else {
        std::cout << "test passed: full queue has not been filled" << "\n";
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
    bool result = q.put(100);

    if (result) {
        std::cout << "oh hey, the insertion works. passed :)" << "\n";
    }
    else {
        std::cout << "noo insertion doesn't work, failed :((" << "\n";
    }

    q.put(400);
    q.put(500);
    q.put(600);

    result = q.take(value);

    if (result) {
        std::cout << "yay! take() is working. passed :)" << "\n";
    }
    else {
        std::cout << "noo something is wrong with take(), failed :(" << "\n";
    }

    q.put(1200);
    q.put(8000);

    result = q.put(56);

    if (result) {
        std::cout << "noo something is wrong with put(), failed :(" << "\n";
    }
    else {
        std::cout << "yay! put() is working. passed :)" << "\n";
    }
    std::cout << "okay, let's test FIFO popping out values now." << "\n";

    q.take(value);
    if (value == 400) {
        std::cout << "expected " << value << ", got " << value << ". sweet :)" << "\n";
    }
    else {
        std::cout << "oh FUCK something's wrong with FIFO god damn it";
    }

    q.take(value);
    if (value == 500) {
        std::cout << "expected " << value << ", got " << value << ". sweet :)" << "\n";
    }
    else {
        std::cout << "oh FUCK something's wrong with FIFO god damn it";
    }

    q.take(value);
    if (value == 600) {
        std::cout << "expected " << value << ", got " << value << ". sweet :)" << "\n";
    }
    else {
        std::cout << "oh FUCK something's wrong with FIFO god damn it";
    }

    q.take(value);
    if (value == 1200) {
        std::cout << "expected " << value << ", got " << value << ". sweet :)" << "\n";
    }
    else {
        std::cout << "oh FUCK something's wrong with FIFO god damn it";
    }

    q.take(value);
    if (value == 8000) {
        std::cout << "expected " << value << ", got " << value << ". sweet :)" << "\n";
    }
    else {
        std::cout << "oh FUCK something's wrong with FIFO god damn it" << "\n";
    }

}


int main() {
    Queue q;
    Queue p;
    Queue r;
    test_empty_queue(p);
    test_ring(p);
    arbitrary_fill(r);
    test_full_queue(r);
}

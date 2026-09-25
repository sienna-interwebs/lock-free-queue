#include<iostream>
#include<vector>

class Queue {
    private:

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
            if (this->count == 0) {
                return false;
            }
            value = this->collection[this->position];
            this->position = (this->position + 1) % collection.size();
            this->count--;
            return true;
        }

        bool put(int value) {
            if (this->count == collection.size()) {
                return false;
            }
            this->collection[this->put_index] = value;
            this->put_index = (this->put_index + 1) % collection.size();
            this->count++;
            return true;
        }

};

int main() {

}

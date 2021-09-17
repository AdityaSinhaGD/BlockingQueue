#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>

template <typename T>
class blocking_queue {

private:
    int _maxSize;
    std::queue<T> _queue;

public:

    blocking_queue(int max_size) : _maxSize(max_size) {}

    void push(T item) {

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        _queue.push(item);
    }

    T pop() {
        T item = _queue.front();
        _queue.pop();
        return item;
    }
};


int main(int argc, const char* argv[]) {
    // insert code here...

    blocking_queue<int> queue(5);

    /*Producer*/
    std::thread t1([&]() {
        for (int i = 0; i < 10; i++) {
            queue.push(i);
        }
        });

    /*Consumer*/
    std::thread t2([&]() {
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        for (int i = 0; i < 10; i++) {
            queue.pop();
            std::cout << "Consumed" << std::endl;
        }
        });

    t1.join();
    t2.join();

    return 0;
}
#include <iostream>
#include <thread>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <queue>

template <typename T>
class blocking_queue {

private:
    std::mutex _queueMutex;
    std::condition_variable cond;
    int _maxSize;
    std::queue<T> _queue;

public:

    blocking_queue(int max_size) : _maxSize(max_size) {}

    void push(T item) {
        std::unique_lock<std::mutex> lock(_queueMutex);
        cond.wait(lock, [this]() {
            return _queue.size() < _maxSize;
            });

        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        _queue.push(item);

        lock.unlock();
        cond.notify_one();
    }

    T front() {
        std::unique_lock<std::mutex> lock(_queueMutex);
        cond.wait(lock, [this]() {
            return !_queue.empty();
            });

        return _queue.front();
    }

    void pop() {

        std::unique_lock<std::mutex> lock(_queueMutex);
        cond.wait(lock, [this]() {
            return !_queue.empty();
            });

        T item = _queue.front();
        _queue.pop();

        lock.unlock();
        cond.notify_one();

    }

    int size() {
        std::lock_guard<std::mutex> lock(_queueMutex);
        return _queue.size();
    }
};


int main(int argc, const char* argv[]) {
    // insert code here...

    blocking_queue<int> queue(5);

    /*Producer*/
    std::thread t1([&]() {
        for (int i = 0; i < 10; i++) {
            std::cout << "Producing:" << i << std::endl;
            queue.push(i);
        }
        });

    /*Consumer*/
    std::thread t2([&]() {
        //std::this_thread::sleep_for(std::chrono::milliseconds(500));
        for (int i = 0; i < 10; i++) {
            int item = queue.front();
            queue.pop();
            std::cout << "Consumed:" <<item<< std::endl;
        }
        });

    t1.join();
    t2.join();

    return 0;
}
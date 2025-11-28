#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

template<typename T>
class ThreadQueue {
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cv;
    bool closed = false;
public:
    ThreadQueue() = default;
    ~ThreadQueue() = default;

    // push an item into the queue (no-op if closed)
    void push(T item) {
        {
            std::scoped_lock lk(m);
            if (closed) return;
            q.push(std::move(item));
        }
        cv.notify_one();
    }

    // pop an item; returns std::nullopt when queue closed and empty
    std::optional<T> pop() {
        std::unique_lock lk(m);
        cv.wait(lk, [&]{ return !q.empty() || closed; });
        if (q.empty()) return std::nullopt;
        T item = std::move(q.front()); q.pop();
        return item;
    }

    // signal no more items will be pushed
    void close() {
        {
            std::scoped_lock lk(m);
            closed = true;
        }
        cv.notify_all();
    }

    bool is_closed() const {
        std::scoped_lock lk(m);
        return closed;
    }
};

#include <queue>
#include <mutex>
#include <condition_variable>
#include <optional>

using namespace std;

template <typename T>
class TQueue
{
private:
    queue<T> m_queue;
    mutex m_mutex;
    condition_variable m_cond;

    bool m_closed = false;

public:
    void Close() {
        {
        unique_lock<mutex> lock(m_mutex);
        m_closed = true;
        }
        m_cond.notify_all();
    }

    void PushItem(T item)
    {
        {
            unique_lock<mutex> lock(m_mutex);
            m_queue.push(item);
        }
        m_cond.notify_one();
    }


    optional<T> Pop()
    {
        unique_lock<mutex> lock(m_mutex);
        m_cond.wait(lock, [this]() { return m_closed || !m_queue.empty();  });

        if (m_queue.empty()) {
            return nullopt; // kolejka zamknięta i pusta
        }
        T item = m_queue.front();
        m_queue.pop();

        return item;
    }

    bool Empty()
    {
        unique_lock<mutex> lock(m_mutex);
        return m_queue.empty();
    }


};

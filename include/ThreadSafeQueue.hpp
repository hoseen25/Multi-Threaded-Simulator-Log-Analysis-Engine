#pragma once

#include <queue>
#include <mutex>
#include <condition_variable>

// Template class allows the queue to hold any data type (e.g., Log strings, tasks, etc.)
template <typename T>
class ThreadSafeQueue {
private:
    std::queue<T> m_queue;           // The standard underlying queue (not thread-safe by default)
    mutable std::mutex m_mutex;      // Mutex to protect shared access to the queue
    std::condition_variable m_cv;    // Signals the background thread when new data arrives

public:
    ThreadSafeQueue() = default;
    
    // Disable copying: It makes no sense to copy a synchronized, live thread-safe queue
    ThreadSafeQueue(const ThreadSafeQueue&) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    // Pushes an item into the queue (Used by the main simulation threads)
    void push(T value) {
        {
            // RAII Lock: Automatically locks the mutex and unlocks it when leaving this scope {}
            std::lock_guard<std::mutex> lock(m_mutex);
            m_queue.push(std::move(value)); // Move semantics prevent costly string copying
        } 
        
        // Notify one waiting thread (the background logger thread) that data is ready
        m_cv.notify_one();
    }

    // Pops an item from the queue (Used by the background Logger thread)
    // This is a BLOCKING function: if the queue is empty, the thread goes to sleep to save CPU
    void pop(T& value) {
        // unique_lock is required because condition_variables need to unlock/relock the mutex dynamically
        std::unique_lock<std::mutex> lock(m_mutex);
        
        // Wait until the queue is not empty. 
        // The lambda function prevents "Spurious Wakeups" (accidental thread wake-ups)
        m_cv.wait(lock, [this]() { return !m_queue.empty(); });
        
        value = std::move(m_queue.front());
        m_queue.pop();
    }

    // Thread-safe check to see if the queue is empty
    bool empty() const {
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }
};

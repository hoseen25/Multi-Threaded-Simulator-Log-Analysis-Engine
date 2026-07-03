#pragma once

#include "ThreadSafeQueue.hpp"
#include <string>
#include <fstream>
#include <thread>
#include <atomic>
#include <iostream>

class Logger {
private:
    ThreadSafeQueue<std::string> m_queue; // Safe queue for logs
    std::ofstream m_file;                 // The log file stream
    std::thread m_logging_thread;         // Background thread for writing logs
    std::atomic<bool> m_running{false};   // Thread safe flag to stop the thread

    // The background function that actually writes logs to the file
    void process_logs() {
        while (m_running || !m_queue.empty()) {
            std::string msg;
            // This blocks (sleeps) if the queue is empty, saving CPU power
            m_queue.pop(msg); 
            
            if (!msg.empty()) {
                m_file << msg << std::endl;
            }
        }
    }

public:
    // Constructor: Opens the log file and starts the background thread
    Logger(const std::string& filename) {
        m_file.open(filename, std::ios::app); // Open in append mode
        if (m_file.is_open()) {
            m_running = true;
            m_logging_thread = std::thread(&Logger::process_logs, this);
        } else {
            std::cerr << "Error: Could not open log file!" << std::endl;
        }
    }

    // Destructor: Safely stops the background thread and closes the file
    ~Logger() {
        m_running = false;
        m_queue.push(""); // Push an empty "wake up" signal to release the pop() block
        
        if (m_logging_thread.joinable()) {
            m_logging_thread.join(); // Wait for the thread to finish cleanly
        }
        if (m_file.is_open()) {
            m_file.close();
        }
    }

    // Public method for any thread to easily drop a log message
    void log(const std::string& message) {
        if (m_running) {
            m_queue.push(message);
        }
    }
};

#include "../include/Logger.hpp"
#include <vector>
#include <iostream>
#include <chrono>

// Function that each simulation thread will run
void run_worker(int worker_id, Logger& logger, int num_logs) {
    for (int i = 1; i <= num_logs; ++i) {
        // Create a log message with the thread's ID and current loop count
        std::string log_msg = "Thread [" + std::to_string(worker_id) + 
                              "] - Log entry #" + std::to_string(i);
        
        logger.log(log_msg);
        
        // Sleep for a tiny bit to simulate realistic task intervals
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

int main() {
    std::cout << "Starting Multi-Threaded Logging Simulation..." << std::endl;

    // Create our logger instance. It will save everything to 'simulation.log'
    Logger logger("simulation.log");

    const int NUM_THREADS = 5;  // Number of threads firing logs at the same time
    const int LOGS_PER_THREAD = 20; // How many logs each thread will send
    
    std::vector<std::thread> threads;

    // Launch all worker threads
    for (int i = 1; i <= NUM_THREADS; ++i) {
        threads.emplace_back(run_worker, i, std::ref(logger), LOGS_PER_THREAD);
    }

    // Wait for all threads to finish their loops cleanly
    for (auto& t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    std::cout << "Simulation finished! Check 'simulation.log' to see results." << std::endl;
    return 0;
}

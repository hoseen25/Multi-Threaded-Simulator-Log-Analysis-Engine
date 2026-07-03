cmake_minimum_required(VERSION 3.10)

# Project Name
project(MultiThreadedLogger VERSION 1.0 LANGUAGES CXX)

# Force C++17 standard for modern features (like std::move, lock_guard, etc.)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include the 'include' directory so the compiler can find Logger.hpp and ThreadSafeQueue.hpp
include_directories(include)

# Define the executable and its source files
add_executable(logger_sim src/main.cpp)

# Link the pthread library (Required for Multi-Threading in Linux/GCC)
target_link_libraries(logger_sim PRIVATE pthread)

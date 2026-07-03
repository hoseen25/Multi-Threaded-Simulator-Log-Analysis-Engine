\# 🚀 High-Performance Multi-Threaded Logging \& SQL Analytics System



A production-grade, asynchronous Full-System project demonstrating low-latency concurrency in \*\*C++\*\*, automated ETL data pipelines in \*\*Python\*\*, and structured data warehousing with relational analytics in \*\*SQL\*\*.



!\[Project Architecture Banner](Photo.png)





\---



\## 🏗️ Architecture Overview



The system is split into two decoupled components to maximize performance and separate concerns:



\[ C++ Multi-threaded Simulator ]

│

▼ (Asynchronous Lock-Free Flow)

\[ simulation.log ]

│

▼ (Automated Python ETL Pipeline)

\[ SQLite Database ] <─── (Analytical Queries \& Aggregations)



1\. \*\*The Core Simulator (C++17):\*\* A high-throughput engine generating massive concurrent events. It avoids slow Disk I/O bottlenecks using a custom asynchronous logger.

2\. \*\*The Analytics Pipeline (Python 3 \& SQL):\*\* An ingestion script that parses raw, unstructured log files using Regular Expressions (Regex) and performs bulk inserts into a structured SQLite database for relational analysis.



\---



\## ⚡ Key Engineering Challenges \& Solutions



\### 1. High-Throughput \& Thread-Safety (C++)

\* \*\*The Problem:\*\* Multiple worker threads writing directly to a log file creates a massive bottleneck due to disk I/O latency and `std::mutex` contention.

\* \*\*The Solution:\*\* Implemented an \*\*Asynchronous Logger\*\* backed by a custom `ThreadSafeQueue`. Worker threads drop messages into the queue in $O(1)$ time and return immediately. A dedicated background thread (`Log Flusher`) uses a `std::condition\_variable` to sleep when the queue is empty and wakes up to flush batches to the disk, saving 100% CPU polling cycles.



\### 2. Zero Memory Leak Warranty

\* \*\*The Problem:\*\* Multi-threaded applications in C++ are prone to race conditions, dangling pointers, and memory leaks.

\* \*\*The Solution:\*\* Developed using modern \*\*RAII\*\* patterns (`std::lock\_guard`, smart ownership transfer with `std::move`). The codebase is strictly verified using \*\*Valgrind Memcheck\*\*, ensuring exactly `0 errors` and `0 leaked bytes` on thread destruction.



\### 3. Fast Data Ingestion \& Indexing (Python \& SQL)

\* \*\*The Problem:\*\* Parsing millions of text strings and updating databases row-by-row is incredibly slow.

\* \*\*The Solution:\*\* Built an efficient \*\*ETL pipeline\*\* using optimized Regex compiled patterns. Utilized SQL `executemany` for high-speed bulk insertions, turning raw log text into actionable, indexed relational data.



\---



\## 🛠️ Tech Stack \& Tools



| Component | Technologies \& Standards | Purpose |

| :--- | :--- | :--- |

| \*\*System Language\*\* | C++17 (Modern Standards) | Low-level Concurrency \& Memory Management |

| \*\*Concurrency\*\* | `std::thread`, `std::mutex`, `std::condition\_variable` | Thread synchronization and Asynchronous execution |

| \*\*Build System\*\* | CMake (3.10+) | Professional cross-platform compilation management |

| \*\*Scripting \& ETL\*\* | Python 3, Regular Expressions (`re`) | Log parsing, automated processing, and DB bridging |

| \*\*Data Warehouse\*\* | SQL (SQLite 3) | Relational storage, data aggregation, and indexing |

| \*\*Diagnostic Tools\*\*| Valgrind (Memcheck) | Memory leaks profiling and stability auditing |



\---



\## 📊 Analytics Sample Output



The Python analyzer automatically extracts metrics using aggregations (`GROUP BY` and `MAX`). 



```text

🐍 Starting Python ETL \& SQL Analysis...

📊 Successfully ingested 200 log entries into SQL database.



\--- 📈 SQL INSIGHTS REPORT ---



\[Log Count Per Thread]

Thread 1: Sent 40 logs

Thread 2: Sent 40 logs

Thread 3: Sent 40 logs ...



\[Last Log Entry Per Thread]

Thread 1 finished at entry #20

Thread 2 finished at entry #20

...

✅ Thread-Safety Verified: 0 dropped messages, perfect consistency.



🚀 How To Run

\# Create build directory and compile

mkdir -p build \&\& cd build

cmake ..

make



\# Execute the simulation (Generates simulation.log)

./logger\_sim

cd ..


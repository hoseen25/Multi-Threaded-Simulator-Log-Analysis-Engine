import re
import sqlite3

# Define the log file path and database name
LOG_FILE = "build/simulation.log"
DB_FILE = "analytics.db"

def run_log_analysis():
    print("🐍 Starting Python ETL & SQL Analysis...")

    # 1. Connect to SQLite Database (creates it if it doesn't exist)
    conn = sqlite3.connect(DB_FILE)
    cursor = conn.cursor()

    # 2. Create a structured SQL table
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS logs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            thread_id INTEGER,
            entry_number INTEGER,
            raw_message TEXT
        )
    """)
    # Clear old data if re-running
    cursor.execute("DELETE FROM logs") 
    conn.commit()

    # 3. Read and Parse the C++ log file using Regex
    # Example line: Thread [3] - Log entry #15
    log_pattern = re.compile(r"Thread \[(\d+)\] - Log entry #(\d+)")
    
    parsed_logs = []
    try:
        with open(LOG_FILE, "r") as file:
            for line in file:
                line = line.strip()
                match = log_pattern.match(line)
                if match:
                    thread_id = int(match.group(1))
                    entry_number = int(match.group(2))
                    parsed_logs.append((thread_id, entry_number, line))
    except FileNotFoundError:
        print(f"❌ Error: Could not find {LOG_FILE}. Did you run the C++ simulator?")
        return

    # 4. Bulk Insert into SQL database (highly efficient)
    cursor.executemany("INSERT INTO logs (thread_id, entry_number, raw_message) VALUES (?, ?, ?)", parsed_logs)
    conn.commit()
    print(f"📊 Successfully ingested {len(parsed_logs)} log entries into SQL database.")

    # 5. Run SQL Queries for Analytical Insights
    print("\n--- 📈 SQL INSIGHTS REPORT ---")

    # Query A: Count logs per thread (Aggregation)
    print("\n[Log Count Per Thread]")
    cursor.execute("SELECT thread_id, COUNT(*) FROM logs GROUP BY thread_id ORDER BY thread_id")
    for row in cursor.fetchall():
        print(f"Thread {row[0]}: Sent {row[1]} logs")

    # Query B: Find the last log entered by each thread (Analytical Max)
    print("\n[Last Log Entry Per Thread]")
    cursor.execute("SELECT thread_id, MAX(entry_number) FROM logs GROUP BY thread_id")
    for row in cursor.fetchall():
        print(f"Thread {row[0]} finished at entry #{row[1]}")

    # Close DB connection
    conn.close()

if __name__ == "__main__":
    run_log_analysis()

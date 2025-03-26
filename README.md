# Project 2 – ThreadSafeDB

This repository contains a multithreaded, networked database server and supporting modules for concurrency, database operations, and testing. Below is an overview of each file, build instructions, and usage guidelines.

---

## Overview

- **Multithreaded Database Server**  
  Listens on a TCP port (default 5000) for incoming connections.  
  Spawns multiple worker threads to process database commands in parallel.

- **Database Module**  
  Stores up to 200 key‐value pairs on disk.  
  Supports write, read, and delete operations.  
  Uses simple file I/O under `/tmp` to store each record.

- **Queue Module**  
  Maintains a thread‐safe work queue for handling incoming client requests.  
  Uses a mutex and condition variable to coordinate enqueuing/dequeuing.

- **Testing**  
  Includes both a compiled testing utility (`dbtest`) and a shell script (`testing.sh`) to automate tests.  
  Verifies correctness under different load conditions and concurrent requests.

---

## File Descriptions

1. **dbserver.c**  
   - Implements the main server logic.  
   - Creates a listening socket on the specified TCP port and accepts client connections.  
   - Spawns multiple worker threads (by default, four) that pick up work items (socket file descriptors) from a shared queue.  
   - Each worker processes read (`R`), write (`W`), delete (`D`), or quit (`Q`) requests using the database module.

2. **database.c**  
   - Implements functions for writing, reading, and deleting records.  
   - Uses a fixed table of up to 200 entries, each stored in a file named `/tmp/data.<index>`.  
   - Provides `db_write`, `db_read`, `db_delete`, and cleanup routines.

3. **database.h**  
   - Declares the data structures (e.g., `db_record`) and function prototypes (e.g., `db_write`, `db_read`) for the database module.  
   - Defines constants such as the maximum number of keys and record name size.

4. **queue.c**  
   - Implements a thread‐safe queue to manage incoming work items.  
   - Uses `pthread_mutex_t` and `pthread_cond_t` to protect the queue and coordinate worker threads.  
   - Provides `enqueue_work`, `dequeue_work`, `queue_init`, `queue_shutdown`, and `queue_cleanup`.

5. **queue.h**  
   - Declares the structures and functions used by `queue.c`.  
   - Defines a `work_item` struct (socket file descriptor plus a pointer to the next item).

6. **testing.sh**  
   - A shell script that runs a series of tests against the server to verify correctness.  
   - Can test basic operations (write, read, delete), run load tests, and handle concurrency scenarios.  
   - Demonstrates usage of `dbtest` and other testing approaches.

7. **Makefile**  
   - Provides rules to build the entire project (`dbserver`, `dbtest`, and any additional tests).  
   - Uses compiler flags (`-ggdb3 -Wall`) for debugging and strict warnings.  
   - Links against `pthread` (`-lpthread`) and zlib (`-lz`), as required for `dbtest`.  
   - Includes `clean` target to remove object files and executables.

---

## Building

In a Unix-like environment (Linux, macOS, or WSL on Windows):

```bash
# Clone or download this repository
cd ThreadSafeDB

# Build everything using the provided Makefile
make

# (Optional) Build clean if needed
make clean
make

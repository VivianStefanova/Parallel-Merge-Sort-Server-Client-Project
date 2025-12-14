# Parallel Merge Sort Server–Client Project (C++ / TCP)

## Description:

This project implements a **TCP-based client–server application** in C++ where the client sends an array of integers to the server, the server sorts the array using **single-threaded or multi-threaded merge sort**, measures execution time, and sends the sorted array and timing results back to the client. The number of threads used during the multi-threaded merge sort is controlled by the client and validated against the system’s hardware concurrency.

The server uses a **thread pool to handle client requests**, allowing multiple client requests to be processed efficiently without creating a new thread per connection. 

The application is developed and run on **Linux (Ubuntu) using Windows Subsystem for Linux (WSL)**.

## Files:

`server.cpp`  
Implements the TCP server.  
- Accepts client connections  
- Uses a **thread pool** to service multiple clients concurrently  
- Receives array size, thread count, and data  
- Performs single-threaded and multi-threaded merge sort  
- Measures execution time  
- Sends timing results and sorted arrays back to the client  

`client.cpp`
Implements the TCP client.  
- Connects to the server using TCP  
- Accepts user input for:
  - Array size  
  - Number of threads  
  - Input method (manual or random)  
- Sends data to the server  
- Receives sorting times and sorted array  

`mergeSort.cpp` / `mergeSort.h`  
Contains merge sort implementations.  
- Single-threaded merge sort  
- Parallel merge sort using `std::thread`
- Thread-limiting and synchronization logic  

`Makefile`  
- Builds the server and client binaries  
- Handles compilation and linking  

## Platform:

- **Operating System:** Ubuntu (Linux)
- **Environment:** Windows Subsystem for Linux (WSL)
- **Compiler:** `g++` (C++20 / C++2a)
- **Concurrency:** POSIX threads (`-pthread`)

## How to Run:
In a terminal running **Linux**:
1. Compile files
    ```bash
    make all
    ```
2. Start server
    ```bash
    ./server.out
    ```
3. In a different terminal star a client
    ```bash
    ./client.out
    ```
    -Enter user data from the terminal and wait for a responce
4. Start as many clients as you like, all on different terminals
    ```bash
    ./client.out
    ```
5. Clen out files
    ```bash
    make clean
    ```

## Key Features:

* **TCP Networking**: TCP communication between client and server
* **Multithreaded server with Thread Pool**: Server uses a fixed pool of worker threads
* **Parallel Merge Sort**: Sorting uses a configurable number of threads
* **Performance Measurement**:
  - Execution time measured using `std::chrono`
* **Input Validation**:
  - Thread count validated against `std::thread::hardware_concurrency()`
  - Supports manual and random array input

## Testing

The project has been tested with: 

### Generated Arrays
- Arrays with **2,000**, **5,000**, and **8,000** elements were generated with random values.
- Both single-threaded and multi-threaded sorting were tested.
- Sorting times were measured and sent back to the client to verify correct execution.

### User Input Array
- An array of **20 elements** was entered manually by the user.
- The program correctly received, sorted, and returned the array to the client.

## Steps for Creating the Project

1. **Create Makefile:**
   - Crate basic file structure
   - Automate building `server.out` and `client.out`.

2. **Implement MergeSort module:**
   - Create `MergeSort.cpp` and `mergeSort.h`.
   - Implement single-threaded merge sort.
   - Implement multi-threaded merge sort with user-specified number of threads.

3. **Implement the server:**
   - Setup TCP socket listening on port `4950`.
   - Accept client connections.
   - Receive array size, array elements, and number of threads.
   - Perform sorting using `MergeSort` functions.
   - Send back sorted array
   - Mesure execution times for sorting algorithms

4. **Implement the client:**
   - Connect to the server using TCP.
   - Allow the user to choose array input: generate random values or enter manually.
   - Allow the user to select the number of threads (validated against hardware concurrency).
   - Send array and thread count to the server.
   - Receive sorted array and sorting times from the server.
   - Display results.

5. **Add thread pool to server**







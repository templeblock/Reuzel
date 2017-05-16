# Reuzel

Reuzel is a tiny C++ thread pool with these features:

  * C++11 syntax and POSIX Threads
  * Simple API
  * Starts all threads on creation of the thread pool
  * Stops and joins all threads on destroy


## Build and example

The thread pool uses C++11 syntax and POSIX Threads so if you compile with g++ on Linux you have to use the flags '-lpthread', '-std=c++11' like this:

    g++ -std=c++11 ThreadPool.cpp Thread.cpp example.cpp -lpthread -o example

Then run the executable like this:
    ./example

if you want to run the test example:
    make
    ./Reuzel


## Basic Usage

1. Include the header file in your source file
2. Create a thread pool, set max size of task queue, and create worker threads
3. Add task to the pool


## Contribution

You are very welcome to contibute!

#include "../src/ThreadPool.h"
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

void print()
{
    std::cout << "current pthread=" << pthread_self() << std::endl;
}

void printString(const std::string &str)
{
    std::cout << str << std::endl;
    usleep(100 * 1000);
}

void test(int maxSize)
{
    std::cout << "Test ThreadPool with max queue size = "
              << maxSize << std::endl;
    Reuzel::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    std::cout << "Adding task" << std::endl;
    pool.addTask(print);
    pool.addTask(print);

    for (int i = 0; i < 100; ++i) {
        std::string tmp("current thread=");
        tmp += std::to_string(pthread_self());
        tmp += " sleep version";
        pool.addTask(std::bind(printString, tmp));
    }

    pool.stop();
}

int main()
{
    test(5);
}

#include "../src/ThreadPool.h"
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>

#include <sys/types.h>

pthread_mutex_t outputMutex= PTHREAD_MUTEX_INITIALIZER;

void print()
{
    pthread_mutex_lock(&outputMutex);
    std::cout << "current pthread = " << pthread_self() << std::endl;
    pthread_mutex_unlock(&outputMutex);
}

void printString(const std::string &taskId)
{
    pthread_mutex_lock(&outputMutex);
    std::cout << "current pthread = " << pthread_self() << " ";
    std::cout << taskId << std::endl;
    usleep(50 * 1000);
    pthread_mutex_unlock(&outputMutex);
}

void test(int maxSize)
{
    std::cout << "Test ThreadPool with max queue size = "
              << maxSize << std::endl;
    Reuzel::ThreadPool pool("MainThreadPool");
    pool.setMaxQueueSize(maxSize);
    pool.start(5);

    pthread_mutex_lock(&outputMutex);
    std::cout << "Adding print task" << std::endl;
    pthread_mutex_unlock(&outputMutex);
    pool.addTask(print);
    pool.addTask(print);

    pthread_mutex_lock(&outputMutex);
    std::cout << "Adding printString task" << std::endl;
    pthread_mutex_unlock(&outputMutex);
    for (int i = 0; i < 100; ++i) {
        std::string taskId("task - ");
        taskId += std::to_string(i);
        pool.addTask(std::bind(printString, taskId));
    }
    sleep(8);  // wait for all threads join

    pool.stop();
}

int main()
{
    test(0);
    test(1);
    test(5);
    test(10);
    test(50);
}

//
// ThreadPool.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "ThreadPool.h"
#include <assert.h>
#include <functional>

using namespace Reuzel;
using std::placeholders::_1;

static void *runInThread(void *arg)
{
    try {
        if ()
    }
}

ThreadPool::ThreadPool(const string &nameArg)
{
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&notEmpty_, NULL);
    pthread_cond_init(&notFull_, NULL);
    name_ = nameArg;
    maxQueueSize_ = 0;
    running_ = false;
}

void ThreadPool::start(int numThreads)
{
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        pthread_t tmp;
        pthread_create(&tmp, NULL, runInThread, NULL);
        threads_.push_back(tmp);
    }

    if (numThreads == 0 && threadInitCallback_) {
        threadInitCallback_();
    }
}

void ThreadPool::stop()
{
    pthread_mutex_lock(&mutex_);
    running_ = false;
    pthread_cond_broadcast(&notEmpty_);
    pthread_mutex_unlock(&mutex_);

    for (auto thread : threads_) {
        pthread_join(thread, NULL);
    }
}

size_t ThreadPool::queueSize() const
{
    pthread_mutex_lock(&mutex_);
    return taskQueue_.size();
    pthread_mutex_unlock(&mutex_);
}

void ThreadPool::run(const Task &task)
{
    if (threads_.empty()) {
        task();
    }
    else {
        pthread_mutex_lock(&mutex_);
        while (isFull()) {
            pthread_cond_wait(&notFull_, &mutex_);
        }
        assert(!isFull());

        taskQueue_.push_back(task);
        pthread_cond_signal(&notEmpty_);

        pthread_mutex_unlock(&mutex_);
    }
}

ThreadPool::Task ThreadPool::take()
{
    pthread_mutex_lock(&mutex_);
    while (taskQueue_.empty() && running_) {
        pthread_cond_wait(&notEmpty_, &mutex_);
    }

    Task task;
    if (!taskQueue_.empty()) {
        task = taskQueue_.front();
        taskQueue_.pop_front();

        if (maxQueueSize_ > 0) {
            pthread_cond_signal(&notFull_);
        }
    }
    pthread_mutex_unlock(&mutex_);

    return task;
}

bool ThreadPool::isFull() const
{
    return maxQueueSize_ > 0 && taskQueue_.size() >= maxQueueSize_;
}

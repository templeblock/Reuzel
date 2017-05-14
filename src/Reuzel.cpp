//
// Reuzel.h
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Reuzel.h"
#include <assert.h>

using namespace Reuzel;

ThreadPool::ThreadPool(const string &nameArg)
{
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&notEmpty_, NULL);
    pthread_cond_init(&notFull_, NULL);
    name_ = nameArg;
    maxQueueSize_ = 0;
    running_  = false;
}

ThreadPool::~ThreadPool()
{
    if (running_) {
        stop();
    }
}

void ThreadPool::start(int numThreads)
{
    assert(threads_.empty());
    running_ = true;
    threads_.reserve(numThreads);
    for (int i = 0; i != numThreads; ++i) {
        pthread_t tmp;
        // FIXME
        //pthread_create(&tmp, NULL, )
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

    for (auto p : threads_) {
        pthread_join(p, NULL);
    }
}

size_t ThreadPool::queueSize() const
{
    return queue_.size();
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

        queue_.push_back(task);

        pthread_cond_signal(&notEmpty_);
        pthread_mutex_unlock(&mutex_);
    }
}

bool ThreadPool::isFull() const
{
    return maxQueueSize_ > 0 && queue_.size() > maxQueueSize_;
}

ThreadPool::Task ThreadPool::take()
{
    pthread_mutex_lock(&mutex_);
    while (queue_.empty() && running_) {
        pthread_cond_wait(&notEmpty_, &mutex_);
    }

    Task task;
    if (!queue_.empty()) {
        task = queue_.front();
        queue_.pop_front();
        if (maxQueueSize_ > 0) {
            pthread_cond_signal(&notFull_);
        }
    }
    pthread_mutex_unlock(&mutex_);

    return task;
}

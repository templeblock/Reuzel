//
// ThreadPool.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "ThreadPool.h"
#include <assert.h>
#include <functional>
#include <algorithm>
#include <exception>
#include <iostream>

using namespace Reuzel;
using std::placeholders::_1;


ThreadPool::ThreadPool(const string &nameArg)
{
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&notEmpty_, NULL);
    pthread_cond_init(&notFull_, NULL);
    name_ = nameArg;
    maxQueueSize_ = 0;
    running_ = false;
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

    for (int i = 0; i < numThreads; ++i) {
        threads_.push_back(Thread(std::bind(&ThreadPool::runInThread, this)));
        threads_[i].start();
    }
    /*
    if (numThreads == 0 && threadInitCallback_) {
        threadInitCallback_();
    }
    */
}

void ThreadPool::stop()
{
    pthread_mutex_lock(&mutex_);
    running_ = false;
    pthread_cond_broadcast(&notEmpty_);
    pthread_mutex_unlock(&mutex_);

    std::for_each(threads_.begin(), threads_.end(),
                   std::bind(&Thread::join, std::placeholders::_1));
}

size_t ThreadPool::queueSize() const
{
    pthread_mutex_lock(&mutex_);
    return taskQueue_.size();
    pthread_mutex_unlock(&mutex_);
}

void ThreadPool::addTask(const Task &task)
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

ThreadPool::Task ThreadPool::takeTask()
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

void ThreadPool::runInThread()
{
    try {
        /*
        if (threadInitCallback_) {
            threadInitCallback_();
        }
        */
        while (running_) {
            Task task(takeTask());
            if (task) {
                task();
            }
        }
    }
    catch (const std::exception &e) {
        std::cerr << "exception caught in ThreadPool" << name_ << std::endl;
        abort();
    }
}

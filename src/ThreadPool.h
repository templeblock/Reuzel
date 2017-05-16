//
// ThreadPool.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef REUZEL_H
#define REUZEL_H

#include <pthread.h>
#include <vector>
#include <deque>
#include <functional>
#include <string>
#include "Thread.h"

using std::string;

namespace Reuzel {
    class ThreadPool {
    public:
        typedef std::function<void ()> Task;

        explicit ThreadPool(const string &nameArg =
                            "Reuzel ThreadPool");
        ~ThreadPool();

        ThreadPool(const ThreadPool&) = delete;
        ThreadPool &operator=(const ThreadPool&) = delete;

        void setMaxQueueSize(int maxSize)
        {
            maxQueueSize_ = maxSize;
        }
        /*
        void setThreadInitCallback(const Task &cb)
        {
            threadInitCallback_ = cb;
        }
        */

        void start(int numThreads);
        void stop();

        const string &name() const
        {
            return name_;
        }

        size_t queueSize() const;

        void addTask(const Task &task);

    private:
        bool isFull() const;
        void runInThread();
        Task takeTask();

        mutable pthread_mutex_t mutex_;
        pthread_cond_t notEmpty_;
        pthread_cond_t notFull_;
        string name_;
        //Task threadInitCallback_;
        std::vector<Thread> threads_;
        std::deque<Task> taskQueue_;
        size_t maxQueueSize_;
        bool running_;
    };
}

#endif

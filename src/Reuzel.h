//
// Reuzel.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef REUZEL_H
#define REUZEL_H

#include <pthread.h>
#include <vector>
#include <deque>
#include <functional>

using std::string;

namespace Reuzel {
    class ThreadPool {
    public:
        typedef std::function<void ()> Task;

        explicit ThreadPool(const string &nameArg = string("ThreadPool"));
    private:
        mutable pthread_mutex_t mutex_;
        pthread_cond_t notEmpty_;
        pthread_cond_t notFull_;
        string name_;
        Task threadInitCallback_;
        std::vector<pthread_t> threads_;
        std::deque<Task> queue_;
        size_t maxQueueSize;
        bool running_;
    };
}

#endif

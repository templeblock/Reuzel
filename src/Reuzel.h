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

namespace Reuzel {
    class ThreadPool {
    public:
        typedef std::function<void ()> Task;

    private:
        mutable pthread_mutex_t mutex_;
        pthread_cond_t notEmpty_;
        pthread_cond_t notFull_;
        std::string name_;
        Task threadInitCallback_;
        std::vector<pthread_t> threads_;
        std::deque<Task> queue_;
        bool running_;
        size_t maxSize;
    };
}

#endif

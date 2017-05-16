//
// Thread.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef THREAD_H
#define THREAD_H

#include <pthread.h>
#include <functional>
#include <assert.h>
#include <exception>
#include <iostream>

namespace Reuzel {
    class Thread {
    public:
        typedef std::function<void ()> ThreadFunc;

        explicit Thread(const ThreadFunc& func);
        ~Thread();

        //FIXME: add copy ctr and copy opr

        void start();
        int join();

        bool started() const
        {
            return started_;
        }

        pthread_t pthreadId() const
        {
            return pthreadId_;
        }

        void runInThread();

    private:
        bool started_;
        bool joined_;
        pthread_t pthreadId_;
        ThreadFunc func_;
    };
}

#endif

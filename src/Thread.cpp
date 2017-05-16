//
// Thread.cpp
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Thread.h"
#include <assert.h>
#include <iostream>

using namespace Reuzel;

static void *startThread(void *arg)
{
    Thread *thread = static_cast<Thread *>(arg);
    thread->runInThread();
    return NULL;
}

Thread::Thread(const ThreadFunc &func)
  : started_(false),
    joined_(false),
    pthreadId_(0)
{
    func_ = func;
}

Thread::~Thread()
{
    if (started_ && !joined_) {
        pthread_detach(pthreadId_);
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;

    if (pthread_create(&pthreadId_, NULL, startThread, this) != 0) {
        started_ = false;
        std::cerr << "Failed in pthread_create" << std::endl;
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(pthreadId_, NULL);
}

void Thread::runInThread()
{
    // FIXME: add assert
    try {
        func_();
    }
    catch (const std::exception &e) {
        std::cerr << "exception caught in Thread "
                  << pthreadId_ << std::endl;
        abort();
    }
}

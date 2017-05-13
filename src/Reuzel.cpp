//
// Reuzel.h
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Reuzel.h"
#include <iostream>
#include <vector>
#include <assert.h>

using namespace Reuzel;

ThreadPool::ThreadPool(const string &nameArg)
{
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&notEmpty_, NULL);
    pthread_cond_init(&notFull_, NULL);
    name_ = nameArg;
    maxQueueSize = 0;
    running_ = false;
}

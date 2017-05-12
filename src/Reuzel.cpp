//
// Reuzel.h
//
// Copyright (c) 2017 Jiawei Feng
//

#include "Reuzel.h"
#include <iostream>
#include <vector>

using namespace Reuzel;

using std::vector;
using std::cout;
using std::endl;

// Class Task static variable
int Task::sign = 0;

// Class ThreadPool static variable
vector<Task *> ThreadPool::task_list;
bool ThreadPool::shutdown;

pthread_mutex_t ThreadPool::mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t ThreadPool::cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t ThreadPool::list_mutex = PTHREAD_MUTEX_INITIALIZER;

Task::Task()
{
    sign = 0;
    task_data = "";
}

Task::~Task()
{
    cout << "Task complete!" << endl;
}


ThreadPool::ThreadPool(int num)
{
    thread_num = num;
    createPool();
}

void ThreadPool::createPool()
{
    ptid = new pthread_t[thread_num];
    for (size_t i = 0; i != thread_num; ++i) {
        pthread_create(&ptid[i], NULL, threadCall, NULL);
        idle.push_back(ptid[i]);
    }
    cout << "Create" << thread_num << "threads successfully" << endl;
}

void *ThreadPool::threadCall(void *data)
{
    
}

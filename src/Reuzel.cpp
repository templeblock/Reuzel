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
pthread_mutex_t ThreadPool::task_list_mutex = PTHREAD_MUTEX_INITIALIZER;

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
    pthread_t this_thread = pthread_self();

    while (1) {
        pthread_mutex_lock(&mutex);
        while (task_list.size() == 0 && !shutdown) {
            pthread_cond_wait(&cond, &mutex);
        }

        if (shutdown) {
            pthread_mutex_unlock(&mutex);
            pthread_mutex_lock(&output_mutex);
            cout << "This thread:" << this_thread << "will exit" << endl;
            pthread_mutex_unlock(&output_mutex);
            pthread_exit(NULL);
        }

        // take out a task
        assert(!task_list.empty());
        auto it = task_list.begin();
        Task *task = *it;
        task_list.erase(it);

        pthread_mutex_unlock(&mutex);

        // task run
        pthread_mutex_lock(&task_list_mutex);
        move2Busy(this_thread);
        pthread_mutex_unlock(&task_list_mutex);
        task->run(this_thread);
        pthread_mutex_lock(&task_list_mutex);
        move2Idle(this_thread);
        pthread_mutex_unlock(&task_list_mutex);
    }
}

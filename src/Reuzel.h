//
// Reuzel.h
//
// Copyright (c) 2017 Jiawei Feng
//

#ifndef REUZEL_H
#define REUZEL_H

#include <vector>
#include <string>
#include <pthread.h>

using std::string;
using std::vector;

namespace Reuzel {
    class Task {
    public:
        Task();
        Task(const string &data) : task_data(data)
        {}
        ~Task();

        void setData(const string &data)
        {
            task_data = data;
        }

        void run(pthread_t pid);
    private:
        static int sign;
        string task_data;
    };

    class ThreadPool {
    public:
        ThreadPool(int num);
        ~ThreadPool();
        void createPool();
        static void *threadCall(void *data);
        static void move2Busy(const pthread_t pid);
        static void move2Idle(const pthread_t pid);

        static pthread_mutex_t output_mutex;
        static vector<pthread_t> busy;
        static vector<pthread_t> idle;
    private:
        static vector<Task *> task_list;
        static bool shutdown;
        int thread_num;
        pthread_t *ptid;
        static pthread_mutex_t mutex;
        static pthread_cond_t cond;
        static pthread_mutex_t task_list_mutex;
    };
}

#endif

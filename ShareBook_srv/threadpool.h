#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <atomic>
#include <iostream>
#include <vector>
#include <functional>
#include <thread>
#include "threadsafequeue.h"

class ThreadPool
{
public:
    ThreadPool();
    ~ThreadPool();
    void submit(std::function<void()> f);

private:
    std::atomic_bool m_done;
    ThreadSafeQueue<std::function<void()>> m_workQueue;
    std::vector<std::thread> m_threads;
//    join_threads m_joiner;

    void worker_thread();
};

#endif // THREADPOOL_H

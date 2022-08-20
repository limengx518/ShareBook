/******************************************************************************
 * Copyright (C) 1997-2022 Free Software Foundation, Inc.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       threadpool.cpp
 * @brief      XXXX Function
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/14
 * @history
 *****************************************************************************/
#include "threadpool.h"

ThreadPool::ThreadPool()
    :m_done(false)
{
    //硬件所能支持的最大线程数量
//    unsigned const thread_count = std::thread::hardware_concurrency();
        unsigned const thread_count = 1000;
    try{
        for(unsigned i = 0; i<thread_count;++i){
            m_threads.push_back(std::thread(&ThreadPool::worker_thread,this));
        }
    }catch(...){
        m_done=true;
        throw;
    }
}

ThreadPool::~ThreadPool()
{
    m_done=true;
    for(auto& item:m_threads){
        if(item.joinable()){
            item.join();
        }
    }
}

void ThreadPool::submit(std::function<void()> f)
{
    m_workQueue.push(std::function<void()>(f));
}

void ThreadPool::worker_thread()
{
    while(!m_done){
        std::function<void()> task;
        if(m_workQueue.try_pop(task)){
            std::cout<<"\n=============开启一个新线程============="<<std::endl;
            task();
        }else std::this_thread::yield();
    }
}

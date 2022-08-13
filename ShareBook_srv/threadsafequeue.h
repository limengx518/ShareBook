/******************************************************************************
 * Copyright 2021-xxxx xxx Co., Ltd.
 * All right reserved. See COPYRIGHT for detailed Information.
 *
 * @file       threadsafequeue.h
 * @brief      线程安全队列
 *
 * @author     李梦雪<2304768518@qq.com> 梁淑贞<moonlsz@163.com> 张宝丹<1395089569@qq.com>
 * @date       2022/08/13
 * @history
 *****************************************************************************/
#ifndef THREADSAFEQUEUE_H
#define THREADSAFEQUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>
#include <memory>

template<typename T>

class ThreadSafeQueue
{
public:
    ThreadSafeQueue(){
    }

    ThreadSafeQueue(ThreadSafeQueue const &other){
        std::lock_guard<std::mutex> lk(other.m_mutex);
        m_queue=other.m_queue;
    }

    void push(T new_value){
        std::lock_guard<std::mutex> lk(m_mutex);
        m_queue.push(new_value);
        m_condition.notify_one();
    }

    void wait_and_pop(T& value){
        std::unique_lock<std::mutex> lk(m_mutex);
        m_condition.wait(m_mutex,[this]{return !m_queue.empty();});
        value=m_queue.front();
        m_queue.pop();
    }

    std::shared_ptr<T> wait_and_pop(){
        std::unique_lock<std::mutex> lk(m_mutex);
        m_condition.wait(m_mutex,[this]{return !m_queue.empty();});
        std::shared_ptr<T> res(std::make_shared<T>(m_queue.front()));
        m_queue.pop();
        return res;
    }
    bool try_pop(T &value){
        std::lock_guard<std::mutex> lk(m_mutex);
        if(m_queue.empty()){
            return false;
        }
        value=m_queue.front();
        m_queue.pop();
        return true;
    }

    std::shared_ptr<T> try_pop(){
        std::lock_guard<std::mutex> lk(m_mutex);
        if(m_queue.empty()){
            return std::shared_ptr<T>();
        }
        std::shared_ptr<T> res(std::make_shared(m_queue.front()));
        m_queue.pop();
        return res;
    }

    bool empty() const{
        std::lock_guard<std::mutex> lk(m_mutex);
        return m_queue.empty();
    }

private:
    std::queue<T> m_queue;
    mutable std::mutex m_mutex;
    std::condition_variable m_condition;
};

#endif // THREADSAFEQUEUE_H

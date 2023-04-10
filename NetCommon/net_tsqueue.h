//
// Created by Emil Tsanev on 31.3.2023 г..
//

#ifndef NETCOMMON_NET_TSQUEUE_H
#define NETCOMMON_NET_TSQUEUE_H

#include "net_common.h"
#include "net_message.h"

namespace olc::net
{
    // Thread-safe queue used for storing the game messages
    template<typename T>
    class tsqueue
    {
    public:
        tsqueue() = default;
        tsqueue(const tsqueue<T>&) = delete;
        virtual ~tsqueue() { clear(); }

        const T& front()
        {
            std::lock_guard<std::mutex> lock(muxQueue); // lock the mutex, using lock_guard
            return deqQueue.front(); // When function is exited thread guard unlock mutex
        }

        void pop()
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            deqQueue.pop_front();
        }

        void push_back(const T& item)
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            deqQueue.push_back(item);
        }

        void push_front(const T& item)
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            deqQueue.push_front(item);
        }

        bool empty()
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            return deqQueue.empty();
        }

        size_t count()
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            return deqQueue.size();
        }

        void clear()
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            deqQueue.clear();
        }

        T pop_front()
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            auto t = std::move(deqQueue.front());
            deqQueue.pop_front();
            return t;
        }

        T pop_back()
        {
            std::lock_guard<std::mutex> lock(muxQueue);
            auto t = std::move(deqQueue.back());
            deqQueue.pop_back();
            return t;
        }

    protected:
        std::mutex muxQueue;
        std::deque<T> deqQueue;
    };

}

#endif //NETCOMMON_NET_TSQUEUE_H
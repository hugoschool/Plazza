#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

namespace plazza {
    template<typename T>
    class SafeQueue {
        public:
            SafeQueue() = default;
            ~SafeQueue() = default;

            void push(T value)
            {
                std::lock_guard lock(_mutex);

                _queue.push(std::move(value));
                _cv.notify_one();
            }

            bool tryPop(T &value)
            {
                std::lock_guard lock(_mutex);

                if (_queue.empty())
                    return false;

                value = std::move(_queue.front());
                _queue.pop();
                return true;
            }

            T pop()
            {
                std::unique_lock lock(_mutex);

                _cv.wait(lock, [this]{ return !_queue.empty(); });

                T value = std::move(_queue.front());
                _queue.pop();
                return value;
            }

            std::size_t size()
            {
                std::lock_guard lock(_mutex);

                return _queue.size();
            }

            bool empty()
            {
                std::lock_guard lock(_mutex);

                return _queue.empty();
            }

            T &front()
            {
                std::lock_guard lock(_mutex);

                return _queue.front();
            }

        private:
            std::mutex _mutex;
            std::condition_variable _cv;

            std::queue<T> _queue;
    };
}

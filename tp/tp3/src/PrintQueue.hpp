#ifndef SRC_PRINTQUEUE_HPP
#define SRC_PRINTQUEUE_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <memory>

#include "LogEvent.hpp"

class PrintQueue {
public:
    PrintQueue();

    std::unique_ptr<LogEvent> pop();
    void push(std::unique_ptr<LogEvent> item);

private:
    std::queue<std::unique_ptr<LogEvent>> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};


#endif // !SRC_PRINTQUEUE_HPP
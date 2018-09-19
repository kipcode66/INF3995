#ifndef SRC_PRINTQUEUE_HPP
#define SRC_PRINTQUEUE_HPP

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "LogEvent.hpp"

class PrintQueue {
public:
    PrintQueue();

    LogEvent* pop();
    void push(LogEvent* item);

private:
    std::queue<LogEvent*> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_condition;
};


#endif // !SRC_PRINTQUEUE_HPP
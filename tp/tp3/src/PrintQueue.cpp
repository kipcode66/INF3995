#include "PrintQueue.hpp"

PrintQueue::PrintQueue()
{ }

std::unique_ptr<LogEvent> PrintQueue::pop() 
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())
    {
        m_condition.wait(mlock);
    }
    auto val = std::move(m_queue.front());
    m_queue.pop();
    return val;
}

void PrintQueue::push(std::unique_ptr<LogEvent> item)
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push(std::move(item));
    m_condition.notify_one();
    mlock.unlock();
}
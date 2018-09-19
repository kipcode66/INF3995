#include "PrintQueue.hpp"

PrintQueue::PrintQueue()
{ }

LogEvent* PrintQueue::pop() 
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    while (m_queue.empty())
    {
        m_condition.wait(mlock);
    }
    auto val = m_queue.front();
    m_queue.pop();
    return val;
}

void PrintQueue::push(LogEvent* item)
{
    std::unique_lock<std::mutex> mlock(m_mutex);
    m_queue.push(item);
    m_condition.notify_one();
    mlock.unlock();
}
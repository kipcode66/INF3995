#ifndef SRC_PRINTHANDLER_HPP
#define SRC_PRINTHANDLER_HPP

#include "PrintQueue.hpp"
#include "LogEvent.hpp"

class PrintHandler {
public:
    PrintHandler(PrintQueue* printQueue);
    ~PrintHandler();

    void print();

private:
    PrintQueue* m_printQueue;
};

#endif // !SRC_PRINTHANDLER_HPP
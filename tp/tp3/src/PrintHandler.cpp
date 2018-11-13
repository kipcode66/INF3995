#include "PrintHandler.hpp"

PrintHandler::PrintHandler(PrintQueue* printQueue)
    : m_printQueue(printQueue)
{}

PrintHandler::~PrintHandler()
{}

void PrintHandler::print() {
    while(1) {
        std::unique_ptr<LogEvent> logEntry = m_printQueue->pop();
        logEntry->print();
    }
}
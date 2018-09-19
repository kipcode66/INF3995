#include "PrintHandler.hpp"

PrintHandler::PrintHandler(PrintQueue* printQueue) 
    : m_printQueue(printQueue) 
{}

PrintHandler::~PrintHandler() 
{}

void PrintHandler::print() {
    while(1) {
        LogEvent* logEntry = m_printQueue->pop();
        logEntry->print();
    } 
}
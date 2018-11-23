#include "PulseOperation.hpp"

namespace elevation {

PulseOperation::PulseOperation(::pa_operation* operation)
    : m_operation(operation)
{ }

PulseOperation::~PulseOperation()
{
    if (m_operation != nullptr) {
        ::pa_operation_unref(m_operation);
    }
}

void PulseOperation::waitUntilCompleteOrFailed() {

}

} // namespace elevation

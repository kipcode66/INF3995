#include "PulseOperation.hpp"

namespace elevation {

PulseOperation::PulseOperation(::pa_operation* operation, ::pa_mainloop* mainloop)
    : m_operation(operation)
    , m_mainloop(mainloop)
{ }

PulseOperation::~PulseOperation()
{
    if (m_operation != nullptr) {
        ::pa_operation_unref(m_operation);
    }
}

void PulseOperation::waitUntilCompleteOrFailed() {
    while (::pa_operation_get_state(m_operation) != PA_OPERATION_DONE) {
        ::pa_mainloop_iterate(m_mainloop, 1, NULL);
    }
}

} // namespace elevation

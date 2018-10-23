#include "PulseDevice.hpp"

namespace elevation {

PulseDevice::PulseDevice()
{
    // TODO
}

PulseDevice::PulseDevice(PulseDevice&& that)
{
    operator=(std::move(that));
}

PulseDevice::~PulseDevice()
{
    cleanup_();
}

PulseDevice& PulseDevice::operator=(PulseDevice&& that) {
    cleanup_();
    this->m_device = that.m_device;
    that .m_device = nullptr;
    return *this;
}

void PulseDevice::cleanup_() {
    if (m_device != nullptr) {
        // TODO Cleanup
    }
}

} // namespace elevation

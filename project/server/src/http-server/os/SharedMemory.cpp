#include "SharedMemory.hpp"

namespace elevation {

SharedMemory::SharedMemory(std::size_t size) { }

SharedMemory::SharedMemory(SharedMemory&& that) { }

SharedMemory::~SharedMemory() { }

SharedMemory& SharedMemory::operator=(SharedMemory&& that) {
    return *this;
}

} // namespace elevation

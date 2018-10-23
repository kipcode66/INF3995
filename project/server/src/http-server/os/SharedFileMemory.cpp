#include "SharedFileMemory.hpp"

#include <sys/mman.h>
#include <sys/stat.h>

namespace elevation {

SharedFileMemory::SharedFileMemory(const std::string& fileName) { }

SharedFileMemory::SharedFileMemory(SharedFileMemory&& that) { }

SharedFileMemory::~SharedFileMemory() { }

SharedFileMemory& SharedFileMemory::operator=(SharedFileMemory&& that) {
    return *this;
}

} // namespace elevation

#include "SharedFileMemory.hpp"

#include <fstream>
#include <stdexcept>
#include <sys/mman.h>
#include <sys/stat.h>

namespace elevation {

SharedFileMemory::SharedFileMemory(const std::string& fileName) { }

SharedFileMemory::SharedFileMemory(SharedFileMemory&& that) { }

SharedFileMemory::~SharedFileMemory() { }

SharedFileMemory& SharedFileMemory::operator=(SharedFileMemory&& that) {
    return *this;
}

std::size_t SharedFileMemory::getFileSize_(const std::string& fileName) const {
    std::ifstream file(fileName, std::ifstream::binary | std::ifstream::ate);
    if (!file.is_open()) {
        throw std::runtime_error(std::string("Could not open ") + fileName);
    }
    std::size_t fileSize = file.tellg();
    return fileSize;
}

} // namespace elevation

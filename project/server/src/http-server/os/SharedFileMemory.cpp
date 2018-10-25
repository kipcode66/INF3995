#include "SharedFileMemory.hpp"

#include <fstream>
#include <stdexcept>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

namespace elevation {

SharedFileMemory::SharedFileMemory(const std::string& fileName)
    : m_buffer(nullptr)
{
    std::size_t fileSize = getFileSize_(fileName);
    openSharedMemory_(fileName, fileSize);
}

SharedFileMemory::SharedFileMemory(SharedFileMemory&& that)
    : m_buffer(nullptr)
    , m_fileSize(0)
{
    operator=(std::move(that));
}

SharedFileMemory::~SharedFileMemory() {
    closeSharedMemory_();
}

SharedFileMemory& SharedFileMemory::operator=(SharedFileMemory&& that) {
    closeSharedMemory_();
    this->m_buffer   = that.m_buffer;
    this->m_fileSize = that.m_fileSize;
    that .m_buffer   = nullptr;
    that .m_fileSize = 0;
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

void SharedFileMemory::openSharedMemory_(const std::string& fileName, std::size_t fileSize) {
    // Open file.
    int fd = ::open(fileName.c_str(), O_RDONLY);
    if (fd < 0) {
        throw std::runtime_error(::strerror(errno));
    }

    // Create shared mem.
    uint8_t* buffer = (uint8_t*)::mmap(NULL, fileSize, PROT_READ, MAP_SHARED, fd, 0);
    ::close(fd); // Don't need to keep the file open ; mmap keeps it open.
    if (buffer == MAP_FAILED) {
        throw std::runtime_error(::strerror(errno));
    }
    m_buffer = buffer;
    m_fileSize = fileSize;
}

void SharedFileMemory::closeSharedMemory_() {
    if (m_buffer != nullptr) {
        if (::munmap(m_buffer, m_fileSize) < 0) {
            throw std::runtime_error(::strerror(errno));
        }
        m_fileSize = 0;
    }
}

} // namespace elevation

#ifndef POSIX_SHAREDFILEMEMORY_HPP
#define POSIX_SHAREDFILEMEMORY_HPP

#include <inttypes.h>
#include <cstddef>
#include <string>

namespace elevation {

class SharedFileMemory {
public:
    explicit SharedFileMemory(const std::string& fileName);
    SharedFileMemory(const SharedFileMemory&) = delete;
    SharedFileMemory(SharedFileMemory&& that);
    virtual ~SharedFileMemory();

    SharedFileMemory& operator=(const SharedFileMemory&) = delete;
    SharedFileMemory& operator=(SharedFileMemory&& that);

    explicit operator       uint8_t*()       { return m_buffer; }
    explicit operator const uint8_t*() const { return m_buffer; }

    uint8_t& operator[] (std::size_t index);
    uint8_t  operator[] (std::size_t index) const;

    std::size_t size() const { return m_fileSize; }

protected:
    std::size_t getFileSize_(const std::string& fileName) const;
    void openSharedMemory_(const std::string& fileName, std::size_t fileSize);
    void closeSharedMemory_();

protected:
    uint8_t* m_buffer;
    std::size_t m_fileSize;
};

} // namespace elevation

#endif // !POSIX_SHAREDFILEMEMORY_HPP

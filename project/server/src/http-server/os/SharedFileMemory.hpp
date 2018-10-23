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

          uint8_t* operator*()       { return m_buffer; }
    const uint8_t* operator*() const { return m_buffer; }

protected:
    std::size_t getFileSize_(const std::string& fileName) const;
    void openSharedMemory_(const std::string& fileName, std::size_t fileSize);
    void closeSharedMemory_();

protected:
    std::size_t m_fileSize;
    uint8_t* m_buffer;
};

} // namespace elevation

#endif // !POSIX_SHAREDFILEMEMORY_HPP

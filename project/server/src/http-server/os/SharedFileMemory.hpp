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

          uint8_t* operator*()       { return m_buffer; }
    const uint8_t* operator*() const { return m_buffer; }

protected:
    std::size_t getFileSize_(const std::string& fileName) const;

protected:
    uint8_t* m_buffer;
};

} // namespace elevation

#endif // !POSIX_SHAREDFILEMEMORY_HPP

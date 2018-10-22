#ifndef POSIX_SHAREDMEMORY_HPP
#define POSIX_SHAREDMEMORY_HPP

#include <inttypes.h>
#include <cstddef>

namespace elevation {

class SharedMemory {
public:
    explicit SharedMemory(std::size_t size);
    SharedMemory(const SharedMemory&) = delete;
    SharedMemory(SharedMemory&& that);
    virtual ~SharedMemory();

    SharedMemory& operator=(const SharedMemory&) = delete;
    SharedMemory& operator=(SharedMemory&& that);

          uint8_t* operator*()       { return m_buffer; }
    const uint8_t* operator*() const { return m_buffer; }

protected:
    uint8_t* m_buffer;
};

} // namespace elevation

#endif // !POSIX_SHAREDMEMORY_HPP

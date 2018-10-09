#include "SocketClosedException.hpp"

namespace elevation {
namespace daemon {

const std::string SocketClosedException::REASON = "Socket closed";

SocketClosedException::SocketClosedException()
{ }

const char* SocketClosedException::what() const noexcept {
    return REASON.c_str();
}

}
}

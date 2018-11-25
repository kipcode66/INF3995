#include "SocketClosedException.hpp"

namespace elevation {

const std::string SocketClosedException::s_REASON = "Socket closed";

SocketClosedException::SocketClosedException()
{ }

const char* SocketClosedException::what() const noexcept {
    return s_REASON.c_str();
}

}

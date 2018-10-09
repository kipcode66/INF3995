#include "ServerUnavailableException.hpp"

namespace elevation {
namespace daemon {

const std::string ServerUnavailableException::REASON = "Socket closed";

ServerUnavailableException::ServerUnavailableException()
{ }

const char* ServerUnavailableException::what() const noexcept {
    return REASON.c_str();
}

}
}

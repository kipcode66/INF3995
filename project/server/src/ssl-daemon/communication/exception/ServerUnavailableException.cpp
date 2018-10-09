#include "ServerUnavailableException.hpp"

namespace elevation {
namespace daemon {

const std::string ServerUnavailableException::REASON = "Server is not available";

ServerUnavailableException::ServerUnavailableException()
{ }

const char* ServerUnavailableException::what() const noexcept {
    return REASON.c_str();
}

}
}

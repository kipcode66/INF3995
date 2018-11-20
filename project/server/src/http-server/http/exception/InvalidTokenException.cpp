#include "InvalidTokenException.hpp"

namespace elevation {

const char* InvalidTokenException::what() const noexcept {
    return "Received an invalid token";
}

} // namespace elevation

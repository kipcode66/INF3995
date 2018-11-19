#include "MissingTokenException.hpp"

namespace elevation {

const char* MissingTokenException::what() const noexcept {
    return "Header \"X-Auth-Token\" missing";
}

} // namespace elevation

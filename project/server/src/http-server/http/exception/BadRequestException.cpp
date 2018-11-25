#include "BadRequestException.hpp"

namespace elevation {

const char* BadRequestException::what() const noexcept {
    return "Malformed request";
}

} // namespace elevation

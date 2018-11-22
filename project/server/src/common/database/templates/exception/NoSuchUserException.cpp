#include "NoSuchUserException.hpp"

namespace elevation {

const char* NoSuchUserException::what() const noexcept {
    return "No Such User";
}

} // namespace elevation


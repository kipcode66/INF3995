#include "NoSuchUserException.hpp"


namespace elevation {

NoSuchUserException::NoSuchUserException()
    : std::runtime_error("No Such User")
{ }

} // namespace elevation


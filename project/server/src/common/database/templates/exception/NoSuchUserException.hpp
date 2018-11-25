#ifndef DATABASE_EXCEPTION_NOSUCHUSEREXCEPTION_HPP
#define DATABASE_EXCEPTION_NOSUCHUSEREXCEPTION_HPP

#include <stdexcept>


namespace elevation {

class NoSuchUserException : public std::runtime_error {
public:
    explicit NoSuchUserException();
};

} // namespace elevation

#endif // !DATABASE_EXCEPTION_NOSUCHUSEREXCEPTION_HPP


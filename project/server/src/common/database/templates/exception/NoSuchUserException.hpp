#ifndef DATABASE_EXCEPTION_NOSUCHUSEREXCEPTION_HPP
#define DATABASE_EXCEPTION_NOSUCHUSEREXCEPTION_HPP

#include <exception>


namespace elevation {

class NoSuchUserException : public std::exception {
public:
    virtual const char* what() const noexcept override;
};

} // namespace elevation

#endif // !DATABASE_EXCEPTION_NOSUCHUSEREXCEPTION_HPP


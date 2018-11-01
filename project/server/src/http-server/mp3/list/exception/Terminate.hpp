#ifndef MP3_LIST_EXCEPTION_TERMINATE
#define MP3_LIST_EXCEPTION_TERMINATE

#include <stdexcept>

namespace elevation {

class Terminate : std::exception {
public:
    virtual const char* what() const noexcept override {
        return "Terminate";
    }
};

}

#endif // !MP3_LIST_EXCEPTION_TERMINATE

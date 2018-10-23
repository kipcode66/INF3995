#include "PulseController.hpp"

namespace elevation {

std::unique_ptr<PulseController> PulseController::s_instance(nullptr);

PulseController& PulseController::getInstance() {
    if (s_instance != nullptr) {
        return *s_instance;
    }
    else {
        throw std::runtime_error("Cannot get PulseController : Not yet created");
    }
}

void PulseController::createInstance() {
    if (s_instance == nullptr) {
        s_instance = std::unique_ptr<PulseController>(new PulseController());
    }
    else {
        throw std::runtime_error("Cannot create PulseController : Already exists");
    }
}

PulseController::PulseController()
{

}

PulseController::~PulseController()
{

}

} // namespace elevation

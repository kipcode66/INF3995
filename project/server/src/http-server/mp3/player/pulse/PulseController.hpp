#ifndef MP3_PLAYER_PULSE_PULSECONTROLLER_HPP
#define MP3_PLAYER_PULSE_PULSECONTROLLER_HPP

#include <memory>

namespace elevation {

class PulseController {
public:
    static PulseController& getInstance();
    static void createInstance();

public:
    ~PulseController();

private:
    PulseController();

protected:
    static std::unique_ptr<PulseController> s_instance;
};

}

#endif // !MP3_PLAYER_PULSE_PULSECONTROLLER_HPP

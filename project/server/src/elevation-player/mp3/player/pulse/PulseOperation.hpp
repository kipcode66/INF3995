#ifndef MP3_PLAYER_PULSE_PULSEOPERATION_HPP
#define MP3_PLAYER_PULSE_PULSEOPERATION_HPP

#include <pulse/pulseaudio.h>

namespace elevation {

/**
 * @brief Assumes ownership of a pulse operation,
 * allowing to easily wait until the operation is complete
 * and to disallocate it when no longer reachable in good old
 * RAII fashion.
 */
class PulseOperation {
public:
    explicit PulseOperation(::pa_operation* operation, ::pa_mainloop* mainloop);
    PulseOperation(const PulseOperation&) = delete;
    PulseOperation(PulseOperation&&) = delete;

    virtual ~PulseOperation();

    PulseOperation& operator=(const PulseOperation&) = delete;
    PulseOperation& operator=(PulseOperation&&) = delete;

    void waitUntilCompletedOrFailed();

protected:
    ::pa_operation* m_operation;
    ::pa_mainloop* m_mainloop;
};

} // namespace elevation

#endif // !MP3_PLAYER_PULSE_PULSEOPERATION_HPP

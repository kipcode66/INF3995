package ca.polymtl.inf3995.client.controller.event

class VolumeChangeRequestEvent(val change: Change, val value: Int? = null): Event {
    companion object {
        enum class Change {
            SET,
            MUTE,
            UNMUTE
        }

        const val EVENT_TYPE = "volume_change_request_event"
    }

    override val type = EVENT_TYPE
}

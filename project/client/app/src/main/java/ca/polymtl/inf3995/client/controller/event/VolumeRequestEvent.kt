package ca.polymtl.inf3995.client.controller.event

class VolumeRequestEvent: Event {
    companion object {
        const val EVENT_TYPE = "volume_request_event"
    }

    override val type = EVENT_TYPE
}

package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.model.Volume

class VolumeChangedEvent(val volume: Volume): Event {
    companion object {
        const val EVENT_TYPE = "volume_changed_event"
    }
    override val type = EVENT_TYPE
}

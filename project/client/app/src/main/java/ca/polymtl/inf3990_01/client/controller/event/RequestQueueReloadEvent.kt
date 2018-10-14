package ca.polymtl.inf3990_01.client.controller.event

class RequestQueueReloadEvent: Event {
    companion object {
        const val EVENT_TYPE = "request_queue_reload"
    }
    override val type = EVENT_TYPE
}
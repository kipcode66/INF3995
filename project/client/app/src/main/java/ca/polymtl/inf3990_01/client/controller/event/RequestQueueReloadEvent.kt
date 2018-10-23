package ca.polymtl.inf3990_01.client.controller.event

/**
 * An event that is fired when we request a refresh of the server's song queue.
 */
class RequestQueueReloadEvent: Event {
    companion object {
        const val EVENT_TYPE = "request_queue_reload"
    }
    override val type = EVENT_TYPE
}
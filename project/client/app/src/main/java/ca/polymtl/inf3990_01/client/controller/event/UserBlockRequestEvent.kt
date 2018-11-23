package ca.polymtl.inf3990_01.client.controller.event

class UserBlockRequestEvent(val username: String, val ip: String, val mac: String): Event {
    companion object {
        const val EVENT_TYPE = "user_block_request_event"
    }
    override val type = EVENT_TYPE
}

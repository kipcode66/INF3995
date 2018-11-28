package ca.polymtl.inf3995.client.controller.event

class UserUnblockRequestEvent(val username: String, val ip: String, val mac: String): Event {
    companion object {
        const val EVENT_TYPE = "user_unblock_request_event"
    }

    override val type = EVENT_TYPE
}
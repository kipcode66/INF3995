package ca.polymtl.inf3995.client.controller.event

class LogoutRequestEvent: Event {
    companion object {
        const val EVENT_TYPE = "logout_request_event"
    }

    override val type = EVENT_TYPE
}
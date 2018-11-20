package ca.polymtl.inf3990_01.client.controller.event

class LoginRequestEvent(
    val login: String,
    val password: String
): Event {
    companion object {
        const val EVENT_TYPE = "login_request_event"
    }
    override val type = EVENT_TYPE
}
package ca.polymtl.inf3990_01.client.controller.event

class PasswordChangeRequestEvent(
    val oldPassword: String,
    val newPassword: String
): Event {
    companion object {
        const val EVENT_TYPE = "login_request_event"
    }
    override val type = EVENT_TYPE
}

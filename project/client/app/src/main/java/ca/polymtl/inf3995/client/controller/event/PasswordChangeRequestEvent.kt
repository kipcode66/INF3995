package ca.polymtl.inf3995.client.controller.event

class PasswordChangeRequestEvent(
    val oldPassword: String,
    val newPassword: String
): Event {
    companion object {
        const val EVENT_TYPE = "password_change_request_event"
    }

    override val type = EVENT_TYPE
}

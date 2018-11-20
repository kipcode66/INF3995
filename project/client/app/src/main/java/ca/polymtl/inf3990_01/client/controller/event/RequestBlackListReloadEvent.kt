package ca.polymtl.inf3990_01.client.controller.event


/**
 * An event that is fired when we request a refresh of the black list of users.
 */
class RequestBlackListReloadEvent: Event {
    companion object {
        const val EVENT_TYPE = "request_black_list_reload"
    }
    override val type = EVENT_TYPE
}
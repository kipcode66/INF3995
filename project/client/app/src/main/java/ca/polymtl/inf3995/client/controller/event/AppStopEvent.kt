package ca.polymtl.inf3995.client.controller.event

/**
 * An event that is fired when the app is no longer visible (when no activities are being shown or when paused).
 */
class AppStopEvent: Event {
    companion object {
        const val EVENT_TYPE = "app_stop"
    }

    override val type = EVENT_TYPE
}
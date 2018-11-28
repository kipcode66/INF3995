package ca.polymtl.inf3995.client.controller.event

/**
 * An event that is fired when the application has finished its initialization phase.
 */
class AppInitEvent: Event {
    companion object {
        const val EVENT_TYPE = "app_init_event"
    }

    override val type: String = EVENT_TYPE
}
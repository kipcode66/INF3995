package ca.polymtl.inf3990_01.client.controller.event

class AppInitEvent: Event {
    companion object {
        const val EVENT_TYPE = "app_init_event"
    }
    override val type: String = EVENT_TYPE
}
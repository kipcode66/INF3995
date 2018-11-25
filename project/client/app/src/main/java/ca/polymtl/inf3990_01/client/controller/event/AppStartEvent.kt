package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.App

/**
 * An event that is fired when the application is first launched.
 */
class AppStartEvent(val app: App): Event {
    companion object {
        const val EVENT_TYPE = "app_start"
    }
    override val type = EVENT_TYPE
}
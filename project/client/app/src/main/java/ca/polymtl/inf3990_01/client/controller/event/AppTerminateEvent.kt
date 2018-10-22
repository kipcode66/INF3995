package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.App

/**
 * An event that is fired when the app is about to be destroyed.
 */
class AppTerminateEvent(val app: App): Event {
    companion object {
        const val EVENT_TYPE = "app_terminate"
    }
    override val type = EVENT_TYPE
}
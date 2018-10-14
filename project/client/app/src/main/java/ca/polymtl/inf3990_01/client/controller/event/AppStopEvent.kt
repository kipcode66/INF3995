package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.App

class AppStopEvent(val app: App): Event {
    companion object {
        val EVENT_TYPE = "app_stop"
    }
    override val type = EVENT_TYPE
}
package ca.polymtl.inf3995.client.controller

import ca.polymtl.inf3995.client.controller.event.AppResumeEvent
import ca.polymtl.inf3995.client.controller.event.AppStopEvent
import ca.polymtl.inf3995.client.controller.event.EventManager

class ActiveActivityTrackingService(private val eventMgr: EventManager) {
    private var activeActivityCounter = 0
    var isActive: Boolean = false
        get() = activeActivityCounter > 0

    @Synchronized
    fun activityStarted() {
        if (activeActivityCounter == 0) {
            eventMgr.dispatchEvent(AppResumeEvent())
        }
        ++activeActivityCounter
    }

    @Synchronized
    fun activityStopped() {
        --activeActivityCounter
        if (activeActivityCounter == 0) {
            eventMgr.dispatchEvent(AppStopEvent())
        }
    }
}
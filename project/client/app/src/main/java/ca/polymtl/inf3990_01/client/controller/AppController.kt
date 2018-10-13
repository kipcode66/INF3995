package ca.polymtl.inf3990_01.client.controller

import android.widget.Toast
import ca.polymtl.inf3990_01.client.controller.event.AppStartEvent
import ca.polymtl.inf3990_01.client.controller.event.EventManager

class AppController(private val eventMgr: EventManager) {

    init {
        eventMgr.addEventListener(AppStartEvent::class.java, this::onAppStart)
    }

    fun onAppStart(event: AppStartEvent) {
        // TODO Start the updating loop
        Toast.makeText(event.app, "App started!", Toast.LENGTH_SHORT)
        eventMgr
    }
}
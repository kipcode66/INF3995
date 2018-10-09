package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.utils.SingletonHolderNoArg

class EventManager private constructor() {
    private val listeners: MutableMap<Class<out Event>, MutableList<(Event) -> Unit>> = mutableMapOf()

    fun <T: Event, L: (T) -> Unit> addEventListener(event: Class<out T>, listener: L) {
        synchronized(listeners) {
            if (listeners[event] == null) {
                listeners[event] = mutableListOf()
            }
            listeners[event]?.add(listener as (Event) -> Unit)
        }
    }

    fun <T: Event, L: (T) -> Unit> removeEventListener(event: Class<out T>, listener: L) {
        synchronized(listeners) {
            listeners[event]?.remove(listener as (Event) -> Unit)
        }
    }

    fun <T: Event> dispatchEvent(evt: T) {
        synchronized(listeners) {
            listeners[evt.javaClass]?.forEach { it(evt) }
        }
    }

    companion object : SingletonHolderNoArg<EventManager>(::EventManager)
}
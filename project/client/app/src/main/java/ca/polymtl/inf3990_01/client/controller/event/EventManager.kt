package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.utils.SingletonHolderNoArg

class EventManager private constructor() {
    private val listeners: MutableMap<Class<out Event>, ArrayList<(Event) -> Unit>> = mutableMapOf()

    @Synchronized fun <T: Event> addEventListener(event: Class<out T>, listener: (T) -> Unit) {
        if (listeners[event] == null) {
            listeners[event] = arrayListOf()
        }
        @Suppress("UNCHECKED_CAST")
        listeners[event]?.add(listener as (Event) -> Unit)
    }

    @Synchronized fun <T: Event> removeEventListener(event: Class<out T>, listener: (T) -> Unit) {
        listeners[event]?.remove(listener)
    }

    @Synchronized fun <T: Event> dispatchEvent(evt: T) {
        listeners[evt.javaClass]?.forEach { it(evt) }
    }

    companion object : SingletonHolderNoArg<EventManager>(::EventManager)
}
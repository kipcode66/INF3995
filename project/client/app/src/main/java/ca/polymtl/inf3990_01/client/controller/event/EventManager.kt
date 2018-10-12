package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.utils.SingletonHolderNoArg

class EventManager private constructor() {
    interface Listener<in T: Event>: (T) -> Unit
    private val listeners: MutableMap<Class<out Event>, ArrayList<Listener<Event>>> = mutableMapOf()

    fun <T: Event> addEventListener(event: Class<out T>, listener: Listener<T>) {
        synchronized(listeners) {
            if (listeners[event] == null) {
                listeners[event] = arrayListOf()
            }
            @Suppress("UNCHECKED_CAST")
            listeners[event]?.add(listener as Listener<Event>)
        }
    }

    fun <T: Event> removeEventListener(event: Class<out T>, listener: Listener<T>) {
        synchronized(listeners) {
            listeners[event]?.remove(listener)
        }
    }

    fun <T: Event> dispatchEvent(evt: T) {
        synchronized(listeners) {
            listeners[evt.javaClass]?.forEach { it(evt) }
        }
    }

    companion object : SingletonHolderNoArg<EventManager>(::EventManager)
}
package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.utils.SingletonHolderNoArg
import java.util.*
import kotlin.collections.HashMap

class EventManager private constructor() {
    init {
    }

    private val listeners: MutableMap<Class<out Event>, MutableList<(Event) -> Unit>> = HashMap()

    fun <T: Event> addEventListener(event: Class<out T>, listener: (T) -> Unit) {
        synchronized(listeners) {
            if (listeners[event] == null) {
                listeners[event] = mutableListOf()
            }
            listeners[event]?.add(listener as (Event) -> Unit)
        }
    }

    fun <T : Event> dispatchEvent(evt: T) {
        synchronized(listeners) {
            listeners[evt.javaClass]?.forEach { it(evt) }
        }
    }

    companion object : SingletonHolderNoArg<EventManager>(::EventManager)
}
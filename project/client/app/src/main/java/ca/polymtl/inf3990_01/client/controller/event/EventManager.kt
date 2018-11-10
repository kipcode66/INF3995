package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.utils.SingletonHolderNoArg
import kotlinx.coroutines.experimental.Job
import kotlinx.coroutines.experimental.joinChildren
import kotlinx.coroutines.experimental.launch
import kotlinx.coroutines.experimental.runBlocking

class EventManager private constructor() {
    private val listeners: MutableMap<Class<out Event>, ArrayList<(Event) -> Unit>> = mutableMapOf()

    @Synchronized fun <T: Event> addEventListener(event: Class<out T>, listener: (T) -> Unit) {
        if (listeners[event] == null) {
            listeners[event] = arrayListOf()
        }
        @Suppress("UNCHECKED_CAST")
        listeners[event]?.add(listener as (Event) -> Unit)
    }

    @Synchronized inline fun <reified T: Event> addEventListener(noinline listener: (T) -> Unit) {
        addEventListener(T::class.java, listener)
    }

    @Synchronized fun <T: Event> removeEventListener(event: Class<out T>, listener: (T) -> Unit) {
        listeners[event]?.remove(listener)
    }

    @Synchronized inline fun <reified T: Event> removeEventListener(noinline listener: (T) -> Unit) {
        removeEventListener(T::class.java, listener)
    }

    @Synchronized fun <T: Event> dispatchEvent(evt: T) {
        runBlocking {
            listeners[evt.javaClass]?.forEach {
                try {
                    launch { it(evt) }
                }
                catch (e: Error) {
                    e.printStackTrace()
                }
            }
            this.coroutineContext[Job]!!.joinChildren()
        }
    }

    companion object : SingletonHolderNoArg<EventManager>(::EventManager)
}
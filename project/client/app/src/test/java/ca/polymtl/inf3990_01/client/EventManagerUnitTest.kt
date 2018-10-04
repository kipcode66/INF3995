package ca.polymtl.inf3990_01.client

import android.util.Log
import ca.polymtl.inf3990_01.client.controller.event.Event
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import org.junit.Test
import org.koin.dsl.module.module
import org.koin.standalone.StandAloneContext.startKoin
import org.koin.standalone.inject
import org.koin.test.KoinTest

class EventManagerUnitTest : KoinTest {
    val eventMgr: EventManager by inject()

    init {
        startKoin(listOf(module {
            single { EventManager.getInstance() }
        }))
    }

    class CustomEvent(override val type: String, val data: Any): Event

    @Test
    fun `can register and call a listener`() {
        // TODO: Test the case.
        var ran = false
        val listener : (CustomEvent) -> Unit = { evt ->
            ran = true
            System.out.println("Hello there!" + evt.data.toString())
        }
        eventMgr.addEventListener(CustomEvent::class.java, listener)
        eventMgr.addEventListener(CustomEvent::class.java) {evt -> System.out.print(evt)}
        eventMgr.dispatchEvent(CustomEvent("my_event", " and here!"))
        assert(ran) {"listener was not run : $ran"}
    }
}
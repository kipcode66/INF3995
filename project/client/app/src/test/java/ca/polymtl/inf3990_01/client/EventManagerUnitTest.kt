package ca.polymtl.inf3990_01.client

import android.content.Context
import ca.polymtl.inf3990_01.client.controller.event.Event
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import org.junit.After
import org.junit.Before
import org.junit.Test
import org.koin.android.ext.koin.with
import org.koin.dsl.module.module
import org.koin.standalone.StandAloneContext.startKoin
import org.koin.standalone.StandAloneContext.stopKoin
import org.koin.standalone.inject
import org.koin.test.KoinTest
import org.mockito.Mockito.mock

class EventManagerUnitTest : KoinTest {
    val eventMgr: EventManager by inject()

    @Before
    fun before() {
        startKoin(listOf(module {
            single { EventManager.getInstance() }
        })) with (mock(Context::class.java))
    }

    @After
    fun after() {
        stopKoin()
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

    @Test
    fun `can register and call a listener with shortened methods`() {
        // TODO: Test the case.
        var ran = false
        val listener : (CustomEvent) -> Unit = { evt ->
            ran = true
            System.out.println("Hello there!" + evt.data.toString())
        }
        eventMgr.addEventListener(listener)
        eventMgr.addEventListener {evt: CustomEvent -> System.out.print(evt)}
        eventMgr.dispatchEvent(CustomEvent("my_event", " and here!"))
        assert(ran) {"listener was not run : $ran"}
    }

    @Test
    fun `can remove a listener`() {
        // TODO: Test the case.
        var ran = false
        val listener : (CustomEvent) -> Unit = { evt ->
            ran = true
            System.out.println("Hello there!" + evt.data.toString())
        }
        eventMgr.addEventListener(CustomEvent::class.java, listener)
        eventMgr.removeEventListener(CustomEvent::class.java, listener)
        eventMgr.dispatchEvent(CustomEvent("my_event", " and here!"))
        assert(!ran) {"listener was run (it shouldn't) : $ran"}
    }

    @Test
    fun `can remove a listener with shortened methods`() {
        // TODO: Test the case.
        var ran = false
        val listener : (CustomEvent) -> Unit = { evt ->
            ran = true
            System.out.println("Hello there!" + evt.data.toString())
        }
        eventMgr.addEventListener(listener)
        eventMgr.removeEventListener(listener)
        eventMgr.dispatchEvent(CustomEvent("my_event", " and here!"))
        assert(!ran) {"listener was run (it shouldn't) : $ran"}
    }
}
package ca.polymtl.inf3990_01.client

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

    @Test
    fun `can register a listener`() {
        // TODO: Test the case.
    }
}
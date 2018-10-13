package ca.polymtl.inf3990_01.client

import android.content.Context
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import org.junit.After
import org.junit.Before
import org.junit.Test
import org.koin.android.ext.koin.with
import org.koin.dsl.module.module
import org.koin.standalone.StandAloneContext
import org.koin.standalone.inject
import org.koin.test.KoinTest
import org.mockito.Mockito


class AppStateUnitTest : KoinTest {

    @Before
    fun before() {
        StandAloneContext.startKoin(listOf(module {
            single { AppStateService() }
        })) with (Mockito.mock(Context::class.java))
    }

    @After
    fun after() {
        StandAloneContext.stopKoin()
    }

    @Test
    fun `initial state is 'user'`() {
        val service: AppStateService by inject()
        assert(service.getState().type == AppStateService.State.User)
    }

    @Test
    fun `change state`() {
        val service: AppStateService by inject()
        service.setState(AppStateService.State.User)
        assert(service.getState().type == AppStateService.State.User)
        service.setState(AppStateService.State.Admin)
        assert(service.getState().type == AppStateService.State.Admin)
    }
}
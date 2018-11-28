package ca.polymtl.inf3995.client

import android.content.Context
import org.junit.After
import org.junit.Before
import org.junit.Test
import org.koin.android.ext.koin.with
import org.koin.standalone.StandAloneContext
import org.koin.test.KoinTest
import org.mockito.Mockito


class TokenManagerServiceUnitTest: KoinTest {

    @Before
    fun before() {
        StandAloneContext.startKoin(listOf(appModule)) with (Mockito.mock(Context::class.java))
    }

    @After
    fun after() {
        StandAloneContext.stopKoin()
    }

    @Test
    fun `can fetch a token`() {

    }
}
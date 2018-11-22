package ca.polymtl.inf3990_01.client

import android.support.test.runner.AndroidJUnit4
import ca.polymtl.inf3990_01.client.controller.rest.HTTPRestClient
import org.junit.Before
import org.junit.Test
import org.junit.runner.RunWith
import org.koin.standalone.StandAloneContext.startKoin

/**
 * Instrumented test, which will execute on an Android device.
 *
 * See [testing documentation](http://d.android.com/tools/testing).
 */
@RunWith(AndroidJUnit4::class)
class HTTPRestClientInstrumentedTest {
    @Before
    fun before() {
        startKoin(listOf())
    }

    @Test
    fun canMakeARequest() {
        val client: HTTPRestClient
    }
}

package ca.polymtl.inf3990_01.client.controller

import android.annotation.SuppressLint
import ca.polymtl.inf3990_01.client.controller.event.AppInitEvent
import ca.polymtl.inf3990_01.client.controller.event.AppStartEvent
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.rest.TokenManagerService
import kotlinx.coroutines.experimental.joinChildren
import kotlinx.coroutines.experimental.launch

class InitializationManager private constructor(
    private val eventMgr: EventManager,
    private val tokenService: TokenManagerService
) {
    companion object {
        /*
         * Warning, the order matters here.
         */
        enum class InitState {
            // States before initialization
            NOT_INITIALIZED,
            // States while initializing
            INITIALIZING,
            // States after initialization
            INITIALIZED,
            CANCELED
        }

        @SuppressLint("StaticFieldLeak")
        @Volatile private var instance: InitializationManager? = null

        fun getInstance(eventMgr: EventManager, tokenService: TokenManagerService): InitializationManager {
            val i = instance
            if (i != null) {
                return i
            }

            return synchronized(this) {
                val i2 = instance
                if (i2 != null) {
                    i2
                } else {
                    val created = InitializationManager(eventMgr, tokenService)
                    instance = created
                    created
                }
            }
        }

        fun getInstanceOrThrow(): InitializationManager {
            return synchronized(this) {
                instance!!
            }
        }

        fun hasInstance(): Boolean {
            return synchronized(this) {
                instance != null
            }
        }
    }
    @Volatile private var initState = InitState.NOT_INITIALIZED
    /**
     * True if the initialization is finished AND not canceled.
     */
    var isInitialized: Boolean = false
        get() = initState >= InitState.INITIALIZED && initState != InitState.CANCELED

    init {
        eventMgr.addEventListener(this::onAppStart)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStart(event: AppStartEvent) {
        initState = InitState.INITIALIZING
        // The list of all initialization jobs
        val initJob = launch {
            // Send a token request and wait for the answer.
            launch {
                // We just want to wait for the request to finish. We don't really care if it succeeded or not.
                tokenService.updateToken()
            }
        }

        // When we finish, notify the rest of the app with an event
        launch {
            // Wait for all jobs to finish
            initJob.joinChildren()
        }.invokeOnCompletion { cause ->
            if (cause == null) {
                initState = InitState.INITIALIZED
                eventMgr.dispatchEvent(AppInitEvent())
            } else {
                initState = InitState.CANCELED
                // TODO: Do something when we can't init the app!
            }
        }
    }
}
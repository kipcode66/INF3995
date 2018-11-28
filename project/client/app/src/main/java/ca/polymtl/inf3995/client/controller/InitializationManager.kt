package ca.polymtl.inf3995.client.controller

import android.annotation.SuppressLint
import android.app.AlarmManager
import android.app.PendingIntent
import android.content.Context
import android.content.Intent
import android.content.SharedPreferences
import android.os.Handler
import android.widget.Toast
import ca.polymtl.inf3995.client.R
import ca.polymtl.inf3995.client.controller.event.AppInitEvent
import ca.polymtl.inf3995.client.controller.event.AppStartEvent
import ca.polymtl.inf3995.client.controller.event.EventManager
import ca.polymtl.inf3995.client.controller.rest.TokenManagerService
import ca.polymtl.inf3995.client.view.QueueActivity
import kotlinx.coroutines.experimental.joinChildren
import kotlinx.coroutines.experimental.launch


class InitializationManager private constructor(
    private val appCtx: Context,
    private val eventMgr: EventManager,
    private val tokenService: TokenManagerService,
    private val preferences: SharedPreferences
) {
    companion object {
        const val CLIENT_NAME_KEY = "client_name"

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
        @Volatile
        private var instance: InitializationManager? = null

        fun getInstance(
            appCtx: Context, eventMgr: EventManager, tokenService: TokenManagerService,
            preferences: SharedPreferences
        ): InitializationManager {
            val i = instance
            if (i != null) {
                return i
            }

            return synchronized(this) {
                val i2 = instance
                if (i2 != null) {
                    i2
                } else {
                    val created = InitializationManager(appCtx, eventMgr, tokenService, preferences)
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

    @Volatile
    private var initState = InitState.NOT_INITIALIZED
    private val preferenceChangeListener = SharedPreferences
        .OnSharedPreferenceChangeListener { _, key ->
            if (key == CLIENT_NAME_KEY) {
                launch {
                    tokenService.updateToken()
                }
            }
        }
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
            // Register handler for username update
            launch {
                preferences.registerOnSharedPreferenceChangeListener(preferenceChangeListener)
            }

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

                Handler(appCtx.mainLooper).post {
                    Toast
                        .makeText(appCtx, appCtx.getString(R.string.error_message_init_canceled), Toast.LENGTH_LONG)
                        .show()
                }
                // We restart the app
                val mStartActivity = Intent(appCtx, QueueActivity::class.java)
                val mPendingIntentId = 123456 // Arbitrary value without a meaning
                val mPendingIntent = PendingIntent
                    .getActivity(appCtx, mPendingIntentId, mStartActivity, PendingIntent.FLAG_CANCEL_CURRENT)
                val mgr = appCtx.getSystemService(Context.ALARM_SERVICE) as AlarmManager
                mgr.set(AlarmManager.RTC, System.currentTimeMillis() + 100, mPendingIntent)
                System.exit(0)
            }
        }
    }
}
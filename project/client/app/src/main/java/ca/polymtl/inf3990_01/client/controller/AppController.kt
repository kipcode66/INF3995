package ca.polymtl.inf3990_01.client.controller

import android.content.Context
import android.content.SharedPreferences
import android.util.Log
import ca.polymtl.inf3990_01.client.controller.event.*
import ca.polymtl.inf3990_01.client.controller.rest.RestRequestService
import ca.polymtl.inf3990_01.client.controller.rest.SecureRestRequestService
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.DataProvider
import kotlinx.coroutines.experimental.Job
import kotlinx.coroutines.experimental.async
import kotlinx.coroutines.experimental.launch
import java.util.concurrent.ScheduledFuture
import java.util.concurrent.ScheduledThreadPoolExecutor
import java.util.concurrent.TimeUnit

class AppController(
        private val eventMgr: EventManager,
        private val restService: RestRequestService,
        private val secureRestService: SecureRestRequestService,
        private val dataProvider: DataProvider,
        private val preferences: SharedPreferences,
        private val localSongController: LocalSongController,
        private val appStateService: AppStateService,
        private val appCtx: Context
) {
    companion object {
        const val QUEUE_PERIOD_KEY = "queue_period"
        const val QUEUE_PERIOD_DEFAULT = 4000L
    }

    private val executor = ScheduledThreadPoolExecutor(1)

    private var reloadQueueJob: Job? = null
    private var loginJob: Job? = null
    private var logoutJob: Job? = null
    private var reloadBlackListJob: Job? = null
    private var swapSongsJob: Job? = null
    private var task: ScheduledFuture<*>? = null

    private val prefChangeListener = SharedPreferences.OnSharedPreferenceChangeListener {sharedPreferences, key ->
        if (key == QUEUE_PERIOD_KEY) {
            task?.cancel(true)
            task = scheduleQueueTask(sharedPreferences)
        }
    }

    init {
        eventMgr.addEventListener(this::onAppInit)
        eventMgr.addEventListener(this::onAppStart)
        eventMgr.addEventListener(this::onAppStop)
        eventMgr.addEventListener(this::reloadQueue)
        eventMgr.addEventListener(this::onSendSong)
        eventMgr.addEventListener(this::onReloadLocalSong)
        eventMgr.addEventListener(this::reloadBlackListUser)
        eventMgr.addEventListener(this::onLoginRequest)
        eventMgr.addEventListener(this::onLogoutRequest)
        eventMgr.addEventListener(this::deleteSong)
        eventMgr.addEventListener(this::swapSongs)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppInit(event: AppInitEvent) {
        eventMgr.removeEventListener(this::onAppInit) // Run it just once
        // Setup the preference listener
        preferences.registerOnSharedPreferenceChangeListener(prefChangeListener)
    }

    /**
     * When the app resumes from inactivity (when starting or coming back from being paused), we start the reload loop.
     */
    @Suppress("UNUSED_PARAMETER")
    private fun onAppStart(event: AppResumeEvent) {
        // Make sure that the previous task was stopped
        task?.cancel(true)
        // Start the updating loop
        task = scheduleQueueTask(preferences)
    }

    /**
     * When the app is no longer visible (when no activities are being shown, when paused), we stop the reload loop.
     */
    @Suppress("UNUSED_PARAMETER")
    private fun onAppStop(event: AppStopEvent) {
        task?.cancel(true)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun reloadQueue(event: RequestQueueReloadEvent) {
        Log.d("AppController", "Reloading the song's queue")
        if (reloadQueueJob?.isCompleted != false) {
            val jobTmp = reloadQueueJob
            reloadQueueJob = async {
                jobTmp?.join()
                val list = if (appStateService.getState().type == AppStateService.State.Admin) {
                    secureRestService.getSongList()
                } else {
                    restService.getSongList()
                }
                // now, we update the model
                dataProvider.setSongQueue(list)
            }
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun reloadBlackListUser(event: RequestBlackListReloadEvent) {
        Log.d("AppController", "Reloading the black list of users")
        if (reloadBlackListJob?.isCompleted != false) {
            val jobTmp = reloadBlackListJob
            reloadBlackListJob = async {
                jobTmp?.join()
                val list = secureRestService.getBlackList()
                // now, we update the model
                dataProvider.setBlackListOfUsers(list)
            }
        }
    }

    private fun scheduleQueueTask(prefs: SharedPreferences): ScheduledFuture<*> {
        return executor.scheduleAtFixedRate({
            eventMgr.dispatchEvent(RequestQueueReloadEvent())
        }, 0, prefs.getString(QUEUE_PERIOD_KEY, "$QUEUE_PERIOD_DEFAULT")?.toLong() ?: QUEUE_PERIOD_DEFAULT, TimeUnit.MILLISECONDS)
    }

    private fun onSendSong(event: SendSongEvent) {
        if (appStateService.getState().type != AppStateService.State.Admin) {
            launch {restService.sendSong(event.song)}
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onReloadLocalSong(event: LocalSongLoadEvent) {
        localSongController.reloadLocalSong(event.activity)
    }

    private fun onLoginRequest(event: LoginRequestEvent) {
        if (loginJob?.isCompleted != false) {
            val jobTmp = loginJob
            loginJob = async {
                jobTmp?.join()
                if (appStateService.getState().type != AppStateService.State.Admin) {
                    secureRestService.login(event.login, event.password)
                }
            }
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onLogoutRequest(event: LogoutRequestEvent) {
        if (logoutJob?.isCompleted != false) {
            val jobTmp = logoutJob
            logoutJob = async {
                jobTmp?.join()
                if (appStateService.getState().type == AppStateService.State.Admin) {
                    secureRestService.logout()
                }
            }
        }
    }

    private fun deleteSong(event: DeleteSongEvent){
        if (appStateService.getState().type == AppStateService.State.Admin) {
            launch {secureRestService.deleteSong(event.song)}
        }
        else {
            launch {restService.deleteSong(event.song)}
        }

    }

    private fun swapSongs(event: SwapSongsRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            if (swapSongsJob?.isCompleted != false) {
                val jobTmp = swapSongsJob
                swapSongsJob = async {
                    jobTmp?.join()
                    secureRestService.swapSongs(event.songs)
                }
            }
        }
    }

}
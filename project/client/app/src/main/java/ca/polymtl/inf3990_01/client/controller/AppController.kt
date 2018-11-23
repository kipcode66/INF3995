package ca.polymtl.inf3990_01.client.controller

import android.content.Context
import android.content.SharedPreferences
import android.os.Handler
import android.os.Looper
import android.util.Log
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.*
import ca.polymtl.inf3990_01.client.controller.rest.RestRequestService
import ca.polymtl.inf3990_01.client.controller.rest.SecureRestRequestService
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.Statistics
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
        private val volumeController: VolumeController,
        private val appCtx: Context
) {
    companion object {
        const val QUEUE_PERIOD_DEFAULT = 4000L
        internal const val NO_DELAY = 0L
    }

    private val executor = ScheduledThreadPoolExecutor(1)

    private var reloadQueueJob: Job? = null
    private var loginJob: Job? = null
    private var logoutJob: Job? = null
    private var reloadBlackListJob: Job? = null
    private var swapSongsJob: Job? = null
    private var volumeRequestJob: Job? = null
    private var volumeChangeRequestJob: Job? = null
    private var task: ScheduledFuture<*>? = null
    private var loadStatisticsJob: Job? = null

    private val prefChangeListener = SharedPreferences.OnSharedPreferenceChangeListener {sharedPreferences, key ->
        when (key) {
            appCtx.getString(R.string.settings_key_queue_refresh_period) -> {
                task?.cancel(true)
                task = scheduleQueueTask(sharedPreferences)
            }
            appCtx.getString(R.string.settings_key_server_address) ->
                appStateService.setState(AppStateService.State.User)
        }
    }

    init {
        eventMgr.addEventListener(this::onAppInit)
        eventMgr.addEventListener(this::onAppResume)
        eventMgr.addEventListener(this::onAppStop)
        eventMgr.addEventListener(this::reloadQueue)
        eventMgr.addEventListener(this::onSendSong)
        eventMgr.addEventListener(this::onReloadLocalSong)
        eventMgr.addEventListener(this::reloadBlackListUser)
        eventMgr.addEventListener(this::onLoginRequest)
        eventMgr.addEventListener(this::onLogoutRequest)
        eventMgr.addEventListener(this::onDeleteSong)
        eventMgr.addEventListener(this::onSwapSongsRequest)
        eventMgr.addEventListener(this::onVolumeRequest)
        eventMgr.addEventListener(this::onVolumeChangeRequest)
        eventMgr.addEventListener(this::loadStatistics)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppInit(event: AppInitEvent) {
        eventMgr.removeEventListener(this::onAppInit) // Run it just once
        // Setup the preference listener
        preferences.registerOnSharedPreferenceChangeListener(prefChangeListener)
    }

    /**
     * When the app resumes from inactivity (when starting or coming back from being paused),
     * we start the reload loop.
     */
    @Suppress("UNUSED_PARAMETER")
    private fun onAppResume(event: AppResumeEvent) {
        // Make sure that the previous task was stopped
        task?.cancel(true)
        // Start the updating loop
        task = scheduleQueueTask(preferences)
    }

    /**
     * When the app is no longer visible (when no activities are being shown, when paused),
     * we stop the reload loop.
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
            eventMgr.dispatchEvent(VolumeRequestEvent())
        },
            NO_DELAY,
            prefs.getString(
                appCtx.getString(R.string.settings_key_queue_refresh_period),
                "$QUEUE_PERIOD_DEFAULT"
            )?.toLong() ?: QUEUE_PERIOD_DEFAULT,
            TimeUnit.MILLISECONDS)
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

    private fun onDeleteSong(event: DeleteSongEvent){
        if (appStateService.getState().type == AppStateService.State.Admin) {
            launch {secureRestService.deleteSong(event.song)}
        }
        else {
            launch {restService.deleteSong(event.song)}
        }

    }

    private fun onSwapSongsRequest(event: SwapSongsRequestEvent) {
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

    @Suppress("UNUSED_PARAMETER")
    private fun onVolumeRequest(event: VolumeRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            if (volumeRequestJob?.isCompleted != false) {
                val jobTmp = volumeRequestJob
                volumeRequestJob = async {
                    jobTmp?.join()
                    val volume = secureRestService.getVolume()
                    if (volumeController.getVolume().level != volume.level) {
                        eventMgr.dispatchEvent(VolumeChangedEvent(volume))
                    }
                }
            }
        }
    }

    private fun onVolumeChangeRequest(event: VolumeChangeRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            if (volumeChangeRequestJob?.isCompleted != false) {
                val jobTmp = volumeChangeRequestJob
                volumeChangeRequestJob = async {
                    jobTmp?.join()
                    when (event.change) {
                        VolumeChangeRequestEvent.Companion.Change.INCREASE -> secureRestService.increaseVolume(event.value!!)
                        VolumeChangeRequestEvent.Companion.Change.DECREASE -> secureRestService.decreaseVolume(event.value!!)
                        VolumeChangeRequestEvent.Companion.Change.MUTE -> secureRestService.muteVolume()
                        VolumeChangeRequestEvent.Companion.Change.UNMUTE -> secureRestService.unmuteVolume()
                    }
                }
            }
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun loadStatistics(event: StatisticsRequestEvent) {
        Log.d("AppController", "loading the statistics")
        if (loadStatisticsJob?.isCompleted != false) {
            val jobTmp = loadStatisticsJob
            loadStatisticsJob = async {
                jobTmp?.join()
                  val statistics : Statistics = secureRestService.getStatistics()
                Handler(Looper.getMainLooper()).post {dataProvider.setStatistics(statistics)}
            }
        }
    }

}
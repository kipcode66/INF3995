package ca.polymtl.inf3995.client.controller

import android.content.Context
import android.content.SharedPreferences
import android.os.Handler
import android.os.Looper
import android.util.Log
import ca.polymtl.inf3995.client.R
import ca.polymtl.inf3995.client.controller.event.*
import ca.polymtl.inf3995.client.controller.event.VolumeChangeRequestEvent.Companion.Change
import ca.polymtl.inf3995.client.controller.rest.RestRequestService
import ca.polymtl.inf3995.client.controller.rest.SecureRestRequestService
import ca.polymtl.inf3995.client.controller.rest.TokenManagerService
import ca.polymtl.inf3995.client.controller.state.AppStateService
import ca.polymtl.inf3995.client.model.DataProvider
import ca.polymtl.inf3995.client.model.Statistics
import ca.polymtl.inf3995.client.model.User
import kotlinx.coroutines.experimental.CoroutineScope
import kotlinx.coroutines.experimental.Job
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
    private val tokenService: TokenManagerService,
    private val initMgr: InitializationManager,
    private val appActiveStateService: ActiveActivityTrackingService,
    private val appCtx: Context
) {
    companion object {
        const val QUEUE_PERIOD_DEFAULT = 4000L
        internal const val NO_DELAY = 0L
    }

    private val executor = ScheduledThreadPoolExecutor(1)
    private var task: ScheduledFuture<*>? = null

    private var reloadQueueJob: Job? = null
    private var loginJob: Job? = null
    private var logoutJob: Job? = null
    private var reloadBlackListJob: Job? = null
    private var swapSongsJob: Job? = null
    private var volumeRequestJob: Job? = null
    private var volumeChangeRequestJob: Job? = null
    private var userBlockRequestJob: Job? = null
    private var userUnblockRequestJob: Job? = null
    private var loadStatisticsJob: Job? = null
    private var changePasswordJob: Job? = null

    private val prefChangeListener = SharedPreferences
        .OnSharedPreferenceChangeListener { sharedPreferences, key ->
            when (key) {
                appCtx.getString(R.string.settings_key_queue_refresh_period) -> {
                    task?.cancel(true)
                    task = scheduleQueueTask(sharedPreferences)
                }
                appCtx.getString(R.string.settings_key_server_address)       ->
                    appStateService.setState(AppStateService.State.User)
                appCtx.getString(R.string.settings_key_server_address)       -> {
                    launch { tokenService.updateToken() }
                }
            }
        }

    init {
        eventMgr.addEventListener(this::onAppInit)
        eventMgr.addEventListener(this::onAppResume)
        eventMgr.addEventListener(this::onAppStop)
        eventMgr.addEventListener(this::onRequestQueueReload)
        eventMgr.addEventListener(this::onSendSong)
        eventMgr.addEventListener(this::onReloadLocalSong)
        eventMgr.addEventListener(this::onRequestBlackListReload)
        eventMgr.addEventListener(this::onLoginRequest)
        eventMgr.addEventListener(this::onLogoutRequest)
        eventMgr.addEventListener(this::onDeleteSong)
        eventMgr.addEventListener(this::onSwapSongsRequest)
        eventMgr.addEventListener(this::onVolumeRequest)
        eventMgr.addEventListener(this::onVolumeChangeRequest)
        eventMgr.addEventListener(this::onStatisticsRequest)
        eventMgr.addEventListener(this::onUserBlockRequest)
        eventMgr.addEventListener(this::onUserUnblockRequest)
        eventMgr.addEventListener(this::onUserBlockRequest)
        eventMgr.addEventListener(this::onChangePasswordRequest)
    }

    private fun <T> queueJob(
        lastJobInQueue: Job? = null, waitIfNotCompleted: Boolean = true,
        block: suspend CoroutineScope.() -> T
    ): Job? {
        if (lastJobInQueue == null || lastJobInQueue.isCompleted || waitIfNotCompleted) {
            return launch {
                lastJobInQueue?.join()
                block.invoke(this)
            }
        }
        return lastJobInQueue
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

    @Suppress("UNUSED_PARAMETER")
    private fun onAppInit(event: AppInitEvent) {
        eventMgr.removeEventListener(this::onAppInit) // Run it just once
        // Setup the preference listener
        preferences.registerOnSharedPreferenceChangeListener(prefChangeListener)
        if (appActiveStateService.isActive) {
            // Make sure that the previous task was stopped
            task?.cancel(true)
            // Start the updating loop
            task = scheduleQueueTask(preferences)
        }
    }

    /**
     * When the app resumes from inactivity (when starting or coming back from being paused),
     * we start the reload loop.
     */
    @Suppress("UNUSED_PARAMETER")
    private fun onAppResume(event: AppResumeEvent) {
        if (initMgr.isInitialized) {
            // Make sure that the previous task was stopped
            task?.cancel(true)
            // Start the updating loop
            task = scheduleQueueTask(preferences)
        }
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
    private fun onRequestQueueReload(event: RequestQueueReloadEvent) {
        Log.d("AppController", "Reloading the song's queue")
        reloadQueueJob = queueJob(reloadQueueJob, false) {
            val list = if (appStateService.getState().type == AppStateService.State.Admin) {
                secureRestService.getSongList()
            } else {
                restService.getSongList()
            }
            // now, we update the model
            dataProvider.setSongQueue(list)
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onRequestBlackListReload(event: RequestBlackListReloadEvent) {
        Log.d("AppController", "Reloading the black list of users")
        reloadBlackListJob = queueJob(reloadBlackListJob, false) {
            val list = secureRestService.getBlackList()
            // now, we update the model
            dataProvider.setBlackListOfUsers(list)
        }
    }

    private fun onSendSong(event: SendSongEvent) {
        if (appStateService.getState().type != AppStateService.State.Admin) {
            launch { restService.sendSong(event.song) }
        }
    }

    private fun onReloadLocalSong(event: LocalSongLoadEvent) {
        localSongController.reloadLocalSong(event.activity)
    }

    private fun onLoginRequest(event: LoginRequestEvent) {
        loginJob = queueJob(loginJob) {
            if (appStateService.getState().type != AppStateService.State.Admin) {
                secureRestService.login(event.login, event.password)
            }
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onLogoutRequest(event: LogoutRequestEvent) {
        logoutJob = queueJob(logoutJob, false) {
            if (appStateService.getState().type == AppStateService.State.Admin) {
                secureRestService.logout()
            }
        }
    }

    private fun onDeleteSong(event: DeleteSongEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            launch { secureRestService.deleteSong(event.song) }
        } else {
            launch { restService.deleteSong(event.song) }
        }
    }

    private fun onSwapSongsRequest(event: SwapSongsRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            swapSongsJob = queueJob(swapSongsJob) { secureRestService.swapSongs(event.songs) }
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onVolumeRequest(event: VolumeRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            volumeRequestJob = queueJob(volumeRequestJob, false) {
                val volume = secureRestService.getVolume()
                if (volumeController.getVolume().level != volume.level) {
                    eventMgr.dispatchEvent(VolumeChangedEvent(volume))
                }
            }
        }
    }

    private fun onVolumeChangeRequest(event: VolumeChangeRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            volumeChangeRequestJob = queueJob(volumeChangeRequestJob) {
                when (event.change) {
                    Change.SET    -> secureRestService.setVolume(event.value!!)
                    Change.MUTE   -> secureRestService.muteVolume()
                    Change.UNMUTE -> secureRestService.unmuteVolume()
                }
            }
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onStatisticsRequest(event: StatisticsRequestEvent) {
        Log.d("AppController", "loading the statistics")
        if (appStateService.getState().type == AppStateService.State.Admin) {
            loadStatisticsJob = queueJob(loadStatisticsJob, false) {
                val statistics: Statistics = secureRestService.getStatistics()
                Handler(Looper.getMainLooper()).post { dataProvider.setStatistics(statistics) }
            }
        }
    }

    private fun onUserBlockRequest(event: UserBlockRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            userBlockRequestJob = queueJob(userBlockRequestJob) {
                secureRestService.blockUser(User(event.mac, event.ip, event.username))
            }
        }
    }

    private fun onUserUnblockRequest(event: UserUnblockRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            userUnblockRequestJob = queueJob(userUnblockRequestJob) {
                secureRestService.unblockUser(User(event.mac, event.ip, event.username))
            }
        }
    }

    private fun onChangePasswordRequest(event: PasswordChangeRequestEvent) {
        if (appStateService.getState().type == AppStateService.State.Admin) {
            changePasswordJob = queueJob(changePasswordJob) {
                if (!event.newPassword.isEmpty()) {
                    secureRestService.changePassword(event.oldPassword, event.newPassword)
                }
            }
        }
    }
}

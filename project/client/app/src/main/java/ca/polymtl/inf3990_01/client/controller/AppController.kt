package ca.polymtl.inf3990_01.client.controller

import android.content.Context
import android.content.SharedPreferences
import android.util.Log
import ca.polymtl.inf3990_01.client.controller.event.*
import ca.polymtl.inf3990_01.client.controller.rest.RestRequestService
import ca.polymtl.inf3990_01.client.presentation.Presenter
import kotlinx.coroutines.experimental.Job
import kotlinx.coroutines.experimental.async
import kotlinx.coroutines.experimental.cancelAndJoin
import kotlinx.coroutines.experimental.launch
import java.util.concurrent.ScheduledFuture
import java.util.concurrent.ScheduledThreadPoolExecutor
import java.util.concurrent.TimeUnit

class AppController(
        private val eventMgr: EventManager,
        private val restService: RestRequestService,
        private val presenter: Presenter,
        private val preferences: SharedPreferences,
        private val localSongController: LocalSongController,
        private val appCtx: Context
) {
    companion object {
        const val QUEUE_PERIOD_KEY = "queue_period"
        const val QUEUE_PERIOD_DEFAULT = 4000L
    }

    private val executor = ScheduledThreadPoolExecutor(1)

    private var reloadQueueJob: Job? = null
    private lateinit var task: ScheduledFuture<*>

    private val prefChangeListener = SharedPreferences.OnSharedPreferenceChangeListener {sharedPreferences, key ->
        if (key == QUEUE_PERIOD_KEY) {
            task.cancel(true)
            task = scheduleQueueTask(sharedPreferences)
        }
    }

    init {
        eventMgr.addEventListener(AppStartEvent::class.java, this::onAppStart)
        eventMgr.addEventListener(AppStopEvent::class.java, this::onAppStop)
        reloadQueue(RequestQueueReloadEvent())
        eventMgr.addEventListener(RequestQueueReloadEvent::class.java, this::reloadQueue)
        eventMgr.addEventListener(SendSongEvent::class.java, this::onSendSong)
        eventMgr.addEventListener(LocalSongLoadEvent::class.java, this::onReloadLocalSong)
    }

    private fun onAppStart(event: AppStartEvent) {
        eventMgr.removeEventListener(AppStartEvent::class.java, this::onAppStart) // Run it just once
        // Start the updating loop
        task = scheduleQueueTask(preferences)
        preferences.registerOnSharedPreferenceChangeListener(prefChangeListener)
    }

    private fun onAppStop(event: AppStopEvent) {
        task.cancel(true)
    }

    private fun reloadQueue(event: RequestQueueReloadEvent) {
        Log.d("AppController", "Reloading the song's queue")
        if (reloadQueueJob?.isCompleted != false) {
            val jobTmp = reloadQueueJob
            reloadQueueJob = async {
                jobTmp?.join()
                val list = restService.getSongList()
                // now, we update the model
                presenter.setQueue(list)
            }
        }
    }

    private fun scheduleQueueTask(prefs: SharedPreferences): ScheduledFuture<*> {
        return executor.scheduleAtFixedRate({
            eventMgr.dispatchEvent(RequestQueueReloadEvent())
        }, 0, prefs.getString(QUEUE_PERIOD_KEY, "$QUEUE_PERIOD_DEFAULT")?.toLong() ?: QUEUE_PERIOD_DEFAULT, TimeUnit.MILLISECONDS)
    }

    private fun onSendSong(event: SendSongEvent) {
        launch {
            restService.sendSong(event.song)
        }
    }

    private fun onReloadLocalSong(event: LocalSongLoadEvent) {
        localSongController.reloadLocalSong()
    }
}
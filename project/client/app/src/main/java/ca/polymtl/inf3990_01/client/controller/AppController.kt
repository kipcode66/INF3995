package ca.polymtl.inf3990_01.client.controller

import android.content.SharedPreferences
import android.util.Log
import android.widget.Toast
import ca.polymtl.inf3990_01.client.controller.event.AppStartEvent
import ca.polymtl.inf3990_01.client.controller.event.AppStopEvent
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.RequestQueueReloadEvent
import ca.polymtl.inf3990_01.client.controller.rest.RestRequestService
import ca.polymtl.inf3990_01.client.presentation.Presenter
import kotlinx.coroutines.experimental.Job
import kotlinx.coroutines.experimental.async
import java.util.concurrent.ScheduledFuture
import java.util.concurrent.ScheduledThreadPoolExecutor
import java.util.concurrent.TimeUnit

class AppController(
        private val eventMgr: EventManager,
        private val restService: RestRequestService,
        private val presenter: Presenter,
        private val preferences: SharedPreferences
) {
    private val executor = ScheduledThreadPoolExecutor(1)

    private var reloadQueueJob: Job? = null
    private lateinit var task: ScheduledFuture<*>

    init {
        eventMgr.addEventListener(AppStartEvent::class.java, this::onAppStart)
        eventMgr.addEventListener(AppStopEvent::class.java, this::onAppStop)
        reloadQueue(RequestQueueReloadEvent())
        eventMgr.addEventListener(RequestQueueReloadEvent::class.java, this::reloadQueue)
    }

    private fun onAppStart(event: AppStartEvent) {
        eventMgr.removeEventListener(AppStartEvent::class.java, this::onAppStart) // Run it just once
        task = scheduleQueueTask(preferences)
        preferences.registerOnSharedPreferenceChangeListener { sharedPreferences, key ->
            if (key == "queue_period") {
                task.cancel(true)
                task = scheduleQueueTask(sharedPreferences)
            }
        }
        
        // TODO Start the updating loop
        Toast.makeText(event.app, "App started!", Toast.LENGTH_LONG).show()
        Log.d("APP", "App started!")
    }

    private fun onAppStop(event: AppStopEvent) {
        task.cancel(true)
    }

    private fun reloadQueue(event: RequestQueueReloadEvent) {
        Log.d("AppController", "Reloading the song's queue")
        val jobTmp = reloadQueueJob
        reloadQueueJob = async {
            jobTmp?.join()
            val list = restService.getSongList()
            presenter.setQueue(list)
        }
    }

    private fun scheduleQueueTask(prefs: SharedPreferences): ScheduledFuture<*> {
        return executor.scheduleAtFixedRate({
            eventMgr.dispatchEvent(RequestQueueReloadEvent())
        }, 0, prefs.getLong("queue_period", 4000), TimeUnit.MILLISECONDS)
    }
}
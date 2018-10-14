package ca.polymtl.inf3990_01.client.controller

import android.util.Log
import android.widget.Toast
import ca.polymtl.inf3990_01.client.controller.event.AppStartEvent
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.RequestQueueReloadEvent
import ca.polymtl.inf3990_01.client.controller.rest.RestRequestService
import ca.polymtl.inf3990_01.client.presentation.Presenter
import kotlinx.coroutines.experimental.Job
import kotlinx.coroutines.experimental.async
import kotlinx.coroutines.experimental.launch

class AppController(private val eventMgr: EventManager, private val restService: RestRequestService, private val presenter: Presenter) {
    private var reloadQueueJob: Job? = null

    init {
        eventMgr.addEventListener(AppStartEvent::class.java, this::onAppStart)
        reloadQueue(RequestQueueReloadEvent())
        eventMgr.addEventListener(RequestQueueReloadEvent::class.java, this::reloadQueue)
    }

    private fun onAppStart(event: AppStartEvent) {
        eventMgr.removeEventListener(AppStartEvent::class.java, this::onAppStart) // Run it just once
        // TODO Start the updating loop
        Toast.makeText(event.app, "App started!", Toast.LENGTH_LONG).show()
        Log.d("APP", "App started!")
    }

    private fun reloadQueue(event: RequestQueueReloadEvent) {
        val jobTmp = reloadQueueJob
        reloadQueueJob = async {
            jobTmp?.join()
            val list = restService.getSongList()
            presenter.setQueue(list)
        }
    }
}
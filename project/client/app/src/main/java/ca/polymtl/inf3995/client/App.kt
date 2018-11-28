package ca.polymtl.inf3995.client

import android.app.Application
import android.support.v7.preference.PreferenceManager
import android.util.Log
import ca.polymtl.inf3995.client.controller.controllerModule
import ca.polymtl.inf3995.client.controller.event.AppStartEvent
import ca.polymtl.inf3995.client.controller.event.AppTerminateEvent
import ca.polymtl.inf3995.client.controller.event.EventManager
import ca.polymtl.inf3995.client.controller.rest.restServiceModule
import ca.polymtl.inf3995.client.model.modelModule
import ca.polymtl.inf3995.client.presentation.presentationModule
import org.koin.android.ext.android.startKoin

class App: Application() {
    private var isStartedInternal = false
    @Suppress("unused")
    val isStarted: Boolean
        get() = isStartedInternal

    lateinit var eventMgr: EventManager

    override fun onCreate() {
        super.onCreate()
        PreferenceManager.setDefaultValues(this, R.xml.preferences, false)
        startKoin(this, listOf(
            appModule,
            controllerModule,
            restServiceModule,
            modelModule,
            presentationModule
        ))

        eventMgr = EventManager.getInstance()
        isStartedInternal = true
        eventMgr.dispatchEvent(AppStartEvent(this))
        Log.d(resources.getString(R.string.app_name), "App started!")
    }

    override fun onTerminate() {
        eventMgr.dispatchEvent(AppTerminateEvent(this))
        super.onTerminate()
    }
}
package ca.polymtl.inf3990_01.client

import android.app.Application
import android.preference.PreferenceManager
import ca.polymtl.inf3990_01.client.controller.controllerModule
import ca.polymtl.inf3990_01.client.controller.rest.restServiceModule
import ca.polymtl.inf3990_01.client.model.modelModule
import ca.polymtl.inf3990_01.client.presentation.presentationModule
import org.koin.android.ext.android.startKoin

class App: Application() {
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
    }
}
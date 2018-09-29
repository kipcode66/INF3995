package ca.polymtl.inf3990_01.client

import android.app.Application
import ca.polymtl.inf3990_01.client.controller.rest.restServiceModule
import ca.polymtl.inf3990_01.client.model.modelModule
import ca.polymtl.inf3990_01.client.presentation.presentationModule
import org.koin.android.ext.android.startKoin

class App: Application() {
    override fun onCreate() {
        super.onCreate()
        startKoin(this, listOf(appModule, restServiceModule, presentationModule, modelModule))
    }
}
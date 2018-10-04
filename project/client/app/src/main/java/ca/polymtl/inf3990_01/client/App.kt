package ca.polymtl.inf3990_01.client

import android.app.Application
import android.content.Context
import ca.polymtl.inf3990_01.client.controller.controllerModule
import ca.polymtl.inf3990_01.client.model.modelModule
import ca.polymtl.inf3990_01.client.presentation.presentationModule
import org.koin.android.ext.android.startKoin
import org.koin.dsl.module.module

class App: Application() {
    override fun onCreate() {
        super.onCreate()
        var context: Context = this
        startKoin(this, listOf(module {
            appModule
            controllerModule
            presentationModule
            modelModule
            single {context}
        }))
    }
}
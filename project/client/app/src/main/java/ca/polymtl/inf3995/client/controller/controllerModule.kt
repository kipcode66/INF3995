package ca.polymtl.inf3995.client.controller

import ca.polymtl.inf3995.client.controller.event.EventManager
import ca.polymtl.inf3995.client.controller.state.AppStateService
import org.koin.dsl.module.module

val controllerModule = module(createOnStart = true) {
    single(createOnStart = true) { InitializationManager.getInstance(get(), get(), get(), get()) }
    single(createOnStart = true) { VolumeController(get(), get(), get()) }
    single { ActiveActivityTrackingService(get()) }
    single { EventManager.getInstance() }
    single { AppStateService(get()) }
    single { AppController(get(), get(), get(), get(), get(), get(), get(), get(), get(), get(), get(), get()) }
    single { LocalSongController(get(), get()) }
}
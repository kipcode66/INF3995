package ca.polymtl.inf3990_01.client.controller

import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import org.koin.dsl.module.module

val controllerModule = module {
    single { EventManager.getInstance() }
    single { AppStateService() }
}
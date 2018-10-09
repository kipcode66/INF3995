package ca.polymtl.inf3990_01.client.controller

import android.content.Context
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.rest.restServiceModule
import org.koin.dsl.module.module

val controllerModule = module {
    single { EventManager.getInstance() }
}
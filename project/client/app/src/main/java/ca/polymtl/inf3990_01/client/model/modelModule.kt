package ca.polymtl.inf3990_01.client.model

import ca.polymtl.inf3990_01.client.model.state.AppStateService
import org.koin.dsl.module.module

val modelModule = module {
    single { AppStateService() }
}
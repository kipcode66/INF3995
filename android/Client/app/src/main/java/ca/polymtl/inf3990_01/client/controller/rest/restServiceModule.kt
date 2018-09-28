package ca.polymtl.inf3990_01.client.controller.rest

import org.koin.dsl.module.module

val restServiceModule = module {
    single { RestRequestService(get()) }
    single { TokenManagerService() }
}
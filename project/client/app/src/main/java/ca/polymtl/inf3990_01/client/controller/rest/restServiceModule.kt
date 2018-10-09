package ca.polymtl.inf3990_01.client.controller.rest

import org.koin.dsl.module.module
import java.net.URL

val restServiceModule = module(createOnStart = true) {
    single { RestRequestService(get()) }
    single { TokenManagerService(get()) }
    factory { HTTPRestClient(get()) }
}
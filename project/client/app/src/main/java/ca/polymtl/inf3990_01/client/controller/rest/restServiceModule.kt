package ca.polymtl.inf3990_01.client.controller.rest

import org.koin.dsl.module.module
import java.net.URL

val restServiceModule = module {
    single { RestRequestService(get()) }
    single { TokenManagerService(get(), get(), get()) }
    factory { HTTPRestClient(get(), get()) }
}
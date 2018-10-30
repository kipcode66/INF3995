package ca.polymtl.inf3990_01.client.controller.rest

import org.koin.dsl.module.module
import java.net.URL

val restServiceModule = module {
    single { RestRequestService(get(), get(), get(), get()) }
    single { TokenManagerService.getInstance(get(), get(), get()) }
    factory { HTTPRestClient(get(), get()) }
}
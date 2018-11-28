package ca.polymtl.inf3995.client.model

import org.koin.dsl.module.module

val modelModule = module {
    single { DataProvider(get(), get()) }
}
package ca.polymtl.inf3995.client

import android.content.SharedPreferences
import android.preference.PreferenceManager
import org.koin.dsl.module.module

val appModule = module {
    single<SharedPreferences> { PreferenceManager.getDefaultSharedPreferences(get()) }
}
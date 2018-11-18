package ca.polymtl.inf3990_01.client.presentation

import android.view.LayoutInflater
import ca.polymtl.inf3990_01.client.model.LocalSongs
import ca.polymtl.inf3990_01.client.model.SongQueue
import org.koin.dsl.module.module

val presentationModule = module {
    single { Presenter(get()) }
    factory { (songQueue: SongQueue, layoutInflater: LayoutInflater) -> SongQueueAdapter(songQueue, layoutInflater, get(), get(), get(), get(),get()) }
    factory { (localSongs: LocalSongs, layoutInflater: LayoutInflater) -> LocalSongAdapter(localSongs, layoutInflater, get(), get(), get(), get(), get())}
}
package ca.polymtl.inf3995.client.presentation

import android.view.LayoutInflater
import android.view.View
import ca.polymtl.inf3995.client.model.LocalSongs
import ca.polymtl.inf3995.client.model.SongQueue
import ca.polymtl.inf3995.client.model.Statistics
import ca.polymtl.inf3995.client.model.UserList
import org.koin.dsl.module.module

val presentationModule = module {
    factory { (songQueue: SongQueue, layoutInflater: LayoutInflater) -> SongQueueAdapter(songQueue, layoutInflater, get(), get(), get(), get(), get()) }
    factory { (localSongs: LocalSongs, layoutInflater: LayoutInflater) -> LocalSongAdapter(localSongs, layoutInflater, get(), get(), get(), get()) }
    factory { (usersList: UserList, layoutInflater: LayoutInflater) -> BlackListAdapter(usersList, layoutInflater, get(), get(), get()) }
    factory { (statistics: Statistics, view: View) -> StatisticsAdapter(statistics, view, get(), get()) }
}

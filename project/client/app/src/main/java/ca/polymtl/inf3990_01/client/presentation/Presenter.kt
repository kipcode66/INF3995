package ca.polymtl.inf3990_01.client.presentation

import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.LocalSongs
import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.model.SongQueue
import java.util.*

/**
 * Observer will receive an argument of type:
 *  - SongQueue : If it's for the server's song queue update
 *  - AppState : If it's an update of the application's state
 */
class Presenter(stateService: AppStateService): Observable() {
    private val songQueue: SongQueue = SongQueue()
    private val localSongs: LocalSongs = LocalSongs()

    init {
        stateService.addObserver(this::onStateUpdate)
    }

    fun setQueue(queue: Collection<Song>) {
        songQueue.clear()
        songQueue.addAll(queue)
        setChanged()
        notifyObservers(songQueue)
    }

    fun setLocalSongs(songs: Collection<LocalSong>) {
        localSongs.clear()
        localSongs.addAll(songs)
        setChanged()
        notifyObservers(localSongs)
    }

    private fun onStateUpdate(o: Observable, arg: Any?) {
        if (o is AppStateService) {
            setChanged()
            notifyObservers(o.getState())
        }
    }
}
package ca.polymtl.inf3990_01.client.presentation

import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.model.SongQueue
import java.util.*

/**
 * Observer will receive an argument of type:
 *  - SongQueue : If it's for the server's song queue update
 */
class Presenter: Observable() {
    private val songQueue: SongQueue = SongQueue()

    fun setQueue(queue: Collection<Song>) {
        songQueue.clear()
        songQueue.addAll(queue)
        setChanged()
        notifyObservers(songQueue)
    }
}
package ca.polymtl.inf3990_01.client.presentation

import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.*
import java.util.*

/**
 * Observer will receive an argument of type:
 *  - SongQueue : If it's for the server's song queue update
 *  - LocalSongs : If it's for the local song's list update
 *  - AppState : If it's an update of the application's state
 */
class Presenter(stateService: AppStateService): Observable() {
    private val songQueue: SongQueue = SongQueue()
    private val blackList: UserList = UserList()

    init {
        stateService.addObserver(this::onStateUpdate)
    }

    fun setQueue(queue: Collection<Song>) {
        songQueue.clear()
        songQueue.addAll(queue)
        setChanged()
        notifyObservers(songQueue)
    }

    fun setBlackListOfUsers(user: Collection<User>){
        blackList.clear()
        blackList.addAll(user)
        setChanged()
        notifyObservers(blackList)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onStateUpdate(o: Observable, arg: Any?) {
        if (o is AppStateService) {
            setChanged()
            notifyObservers(o.getState())
        }
    }
    fun getSongs(): SongQueue {
        return songQueue
    }
}
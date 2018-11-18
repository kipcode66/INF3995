package ca.polymtl.inf3990_01.client.model

import java.util.*

class DataProvider {
    enum class LocalSongSendState {
        NOT_SENT,
        SENDING,
        WAITING_FOR_SERVER_QUEUE,
        SENT;
    }

    private class ChangeableObservable: Observable() {
        fun makeChanged() {
            super.setChanged()
        }
    }

    private val localSongs: LocalSongs = LocalSongs()
    private val localSongsObservable = ChangeableObservable()
    private val blackListObservable = ChangeableObservable()
    private val songStates: HashMap<LocalSong, LocalSongSendState> = hashMapOf()
    private val songStatesObservable = ChangeableObservable()
    private val blackList: UserList = UserList()

    @Synchronized operator fun set(song: LocalSong, state: LocalSongSendState) {
        val s = songStates.keys.find { it.compareTo(song) == 0 }
        synchronized(songStates) {
            if (s !== null) {
                songStates[s] = state
            }
            else {
                songStates[song] = state
            }
        }
        songStatesObservable.makeChanged()
        songStatesObservable.notifyObservers(s ?: song)
    }

    operator fun get(song: LocalSong) =
        songStates[songStates.keys.find { it.compareTo(song) == 0 }] ?: LocalSongSendState.NOT_SENT

    fun observeLocalSongSendStates(o: Observer) {
        songStatesObservable.addObserver(o)
    }

    fun unobserveLocalSongSendStates(o: Observer) {
        songStatesObservable.deleteObserver(o)
    }

    fun setLocalSongs(songs: Collection<LocalSong>) {
        localSongs.clear()
        localSongs.addAll(songs)
        localSongsObservable.makeChanged()
        localSongsObservable.notifyObservers(localSongs)
    }
    fun setBlackListOfUsers(user: Collection<User>){
        blackList.clear()
        blackList.addAll(user)
        blackListObservable.makeChanged()
        blackListObservable.notifyObservers(blackList)
    }
    fun getLocalSongs(): LocalSongs = localSongs.clone() as LocalSongs

    fun observeLocalSongs(o: Observer) {
        localSongsObservable.addObserver(o)
    }

    fun unobserveLocalSongs(o: Observer) {
        localSongsObservable.deleteObserver(o)
    }

}
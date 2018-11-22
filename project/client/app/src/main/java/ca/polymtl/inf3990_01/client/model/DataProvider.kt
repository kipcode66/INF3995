package ca.polymtl.inf3990_01.client.model

import android.content.SharedPreferences
import ca.polymtl.inf3990_01.client.controller.event.AppInitEvent
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import java.util.*

class DataProvider(
    eventMgr: EventManager,
    settings: SharedPreferences
) {
    companion object {
        private class ChangeableObservable: Observable() {
            fun makeChanged() {
                super.setChanged()
            }
        }
    }

    enum class LocalSongSendState {
        NOT_SENT,
        SENDING,
        WAITING_FOR_SERVER_QUEUE,
        SENT;
    }

    private val songStates: HashMap<LocalSong, LocalSongSendState> = hashMapOf()
    private val songStatesObservable = ChangeableObservable()
    private val songQueue: SongQueue = SongQueue()
    private val songQueueObservable = ChangeableObservable()
    private val localSongs: LocalSongs = LocalSongs()
    private val localSongsObservable = ChangeableObservable()
    private val blackList: UserList = UserList()
    private val blackListObservable = ChangeableObservable()
    private val settingObservable = ChangeableObservable()

    init {
        eventMgr.addEventListener<AppInitEvent> {settings.registerOnSharedPreferenceChangeListener(this::onSettingsChanged)}
    }

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

    fun observeLocalSongSendStates(o: Observer) = songStatesObservable.addObserver(o)
    fun unobserveLocalSongSendStates(o: Observer) = songStatesObservable.deleteObserver(o)

    fun setSongQueue(songs: Collection<Song>) {
        songQueue.clear()
        songQueue.addAll(songs)
        songQueueObservable.makeChanged()
        songQueueObservable.notifyObservers(songQueue)
    }
    fun getSongQueue(): SongQueue = songQueue.clone() as SongQueue
    fun observeSongQueue(o: Observer) = songQueueObservable.addObserver(o)
    fun unobserveSongQueue(o: Observer) = songQueueObservable.deleteObserver(o)

    fun setLocalSongs(songs: Collection<LocalSong>) {
        localSongs.clear()
        localSongs.addAll(songs)
        localSongsObservable.makeChanged()
        localSongsObservable.notifyObservers(localSongs)
    }
    fun getLocalSongs(): LocalSongs = localSongs.clone() as LocalSongs
    fun observeLocalSongs(o: Observer) = localSongsObservable.addObserver(o)
    fun unobserveLocalSongs(o: Observer) = localSongsObservable.deleteObserver(o)

    fun setBlackListOfUsers(user: Collection<User>) {
        blackList.clear()
        blackList.addAll(user)
        blackListObservable.makeChanged()
        blackListObservable.notifyObservers(blackList)
    }
    fun getBlackList(): UserList = blackList.clone() as UserList
    fun observeBlackList(o: Observer) = blackListObservable.addObserver(o)
    fun unobserveBlackList(o: Observer) = blackListObservable.deleteObserver(o)

    private fun onSettingsChanged(settings: SharedPreferences, key: String) {
        settingObservable.makeChanged()
        settingObservable.notifyObservers(key)
    }
    fun observeSettings(o: Observer) = settingObservable.addObserver(o)
    fun unobserveSettings(o: Observer) = settingObservable.deleteObserver(o)

}
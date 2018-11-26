package ca.polymtl.inf3990_01.client.presentation

import android.content.Context
import android.os.Handler
import android.os.Looper
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.SendSongEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.LocalSongs
import ca.polymtl.inf3990_01.client.model.SongQueue
import ca.polymtl.inf3990_01.client.utils.Misc
import kotlinx.android.synthetic.main.local_song.view.*
import java.util.*

class LocalSongAdapter(
        private val localSongs: LocalSongs,
        private val layoutInflater: LayoutInflater,
        private val appCtx: Context,
        private val eventMgr: EventManager,
        private val stateService: AppStateService,
        private val dataProvider: DataProvider
): BaseAdapter() {
    companion object {
        internal const val MAX_CONCURRENT_SENDING = 3L
    }

    private var ownedSongs = getUpdatedOwnerSongsQueue()

    init {
        stateService.addObserver(this::onStateChange)
        dataProvider.observeSongQueue(Observer(this::onSongQueueChange))
        dataProvider.observeLocalSongSendStates(Observer(this::onLocalSongSendStateChange))
        dataProvider.observeLocalSongs(Observer(this::onLocalSongsChange))
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onLocalSongSendStateChange(o: Observable, arg: Any?) {
        Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onLocalSongsChange(o: Observable, arg: Any?) {
        if (arg is LocalSongs) {
            localSongs.clear()
            localSongs.addAll(arg)
            Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
        }
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onSongQueueChange(o: Observable, arg: Any?) {
        ownedSongs = getUpdatedOwnerSongsQueue()
        updateLocalSongSendState(arg as SongQueue)
        Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onStateChange(o: Observable, arg: Any?) {
        Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
    }

    private fun getUpdatedOwnerSongsQueue () = dataProvider.getSongQueue().filter { song -> song.sentBy == null }

    private fun updateLocalSongSendState(songs: SongQueue) {
        for (s in songs) {
            val song = localSongs.find { it.title == s.title && it.authorName == s.authorName }
            if (song !== null) {
                dataProvider[song] = DataProvider.LocalSongSendState.SENT
            }
        }
        localSongs.forEach { song ->
            if (dataProvider[song] >= DataProvider.LocalSongSendState.WAITING_FOR_SERVER_QUEUE
                    && !ownedSongs.any { s -> s.title == song.title && s.authorName == song.authorName }) {
                dataProvider[song] = DataProvider.LocalSongSendState.NOT_SENT
            }
        }
    }

    override fun getView(position: Int, v: View?, viewGroup: ViewGroup?): View {
        val view = v ?: layoutInflater.inflate(R.layout.local_song, viewGroup, false)
        val song = this.localSongs[position]
        view.songName.text = song.title
        view.author.text = song.authorName
        val duration : String = Misc.formatTime(song.durationMS.toLong())
        view.duration.text = appCtx.getString(R.string.song_item_duration_format, duration)
        val isAdmin = stateService.getState().type == AppStateService.State.Admin
        val isSongInQueue = ownedSongs.any { s -> s.title == song.title && s.authorName == song.authorName }
        view.send.isEnabled = ownedSongs.size < 5 && !isSongInQueue && !isAdmin && (dataProvider[song] <= DataProvider.LocalSongSendState.NOT_SENT)
        view.send.text = when (dataProvider[song]) {
            DataProvider.LocalSongSendState.SENDING -> appCtx.getString(R.string.local_song_sendButton_label_sending)
            DataProvider.LocalSongSendState.WAITING_FOR_SERVER_QUEUE -> appCtx.getString(R.string.local_song_sendButton_label_sending)
            DataProvider.LocalSongSendState.SENT -> appCtx.getString(R.string.local_song_sendButton_label_sent)
            else -> appCtx.getString(R.string.local_song_sendButton_label_send)
        }

        view.send.setOnClickListener {
            val nSendingSong = localSongs.map { song -> dataProvider[song] }.filter { state -> state > DataProvider.LocalSongSendState.NOT_SENT && state < DataProvider.LocalSongSendState.SENT }.size
            if (nSendingSong < MAX_CONCURRENT_SENDING) {
                dataProvider[song] = DataProvider.LocalSongSendState.SENDING
                eventMgr.dispatchEvent(SendSongEvent(song))
            }
            else {
                Handler(Looper.getMainLooper()).post {
                    Toast.makeText(appCtx, appCtx.getString(R.string.error_message_too_many_concurrent_song_sent, MAX_CONCURRENT_SENDING), Toast.LENGTH_LONG).show()
                }
            }
        }
        return view
    }

    override fun getItemId(p: Int): Long {
        return p.toLong()
    }

    override fun getCount(): Int {
        return this.localSongs.size
    }
    override fun getItem(item: Int): Any {
        return this.localSongs[item]
    }
}
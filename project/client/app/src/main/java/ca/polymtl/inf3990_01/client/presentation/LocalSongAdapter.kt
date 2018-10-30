package ca.polymtl.inf3990_01.client.presentation
import android.content.Context
import android.os.Handler
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.SendSongEvent
import ca.polymtl.inf3990_01.client.model.LocalSongs
import ca.polymtl.inf3990_01.client.model.SongQueue
import kotlinx.android.synthetic.main.local_song.view.*
import kotlinx.android.synthetic.main.local_song.view.send
import java.util.*


class LocalSongAdapter(
        private val localSongs: LocalSongs,
        private val layoutInflater: LayoutInflater,
        private val appCtx: Context,
        private val eventMgr: EventManager,
        private val presenter: Presenter
): BaseAdapter() {
    private var ownedSongs = getUpdatedOwnerSongsQueue()

    init {
        presenter.addObserver(Observer(this::onPresenter))
    }

    private fun onPresenter(o: Observable, arg: Any?) {
        if (arg is LocalSongs) {
            localSongs.clear()
            localSongs.addAll(arg)
            Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
        }
        else if (arg is SongQueue) {
            ownedSongs = getUpdatedOwnerSongsQueue()
            Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
        }
    }
    private fun getUpdatedOwnerSongsQueue () = presenter.getSongs().filter { song -> song.sentBy == null }

    override fun getView(postion: Int, v: View?, viewGroup: ViewGroup?): View {
        val view = v ?: layoutInflater.inflate(R.layout.local_song, viewGroup, false)
        val song = this.localSongs[postion]
        view.songName.text = song.title
        view.author.text = song.authorName

        view.send.isEnabled = !(ownedSongs.size >= 5)

        view.send.setOnClickListener {
            eventMgr.dispatchEvent(SendSongEvent(song))
        }
        return view
    }

    override fun getItemId(p: Int): Long {
        return  p.toLong()
    }

    override fun getCount(): Int {
        return this.localSongs.size
    }
    override fun getItem(item: Int): Any {
        return this.localSongs[item]
    }
}
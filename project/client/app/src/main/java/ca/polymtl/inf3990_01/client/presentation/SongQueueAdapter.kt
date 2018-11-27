package ca.polymtl.inf3990_01.client.presentation

import android.content.Context
import android.content.SharedPreferences
import android.os.Handler
import android.util.Log
import android.view.*
import android.widget.BaseAdapter
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.DeleteSongEvent
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.SwapSongsRequestEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.model.SongQueue
import ca.polymtl.inf3990_01.client.utils.Misc
import kotlinx.android.synthetic.main.server_song.view.*
import java.util.*

class SongQueueAdapter(
        private val songQueue: SongQueue,
        private val layoutInflater: LayoutInflater,
        private val stateService: AppStateService,
        private val appCtx: Context,
        private val preferences: SharedPreferences,
        private val eventMgr: EventManager,
        private val dataProvider: DataProvider
): BaseAdapter() {
    companion object {
        private enum class State {
            INITIAL,
            SWAP_SELECTION;
        }
    }

    private var state = State.INITIAL
    private var firstSelection: Song? = null

    init {
        dataProvider.observeSongQueue(Observer(this::onSongQueueChange))
        stateService.addObserver(this::onAppStateChange)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStateChange( o: Observable, arg: Any?) {
        Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onSongQueueChange(o: Observable, arg: Any?) {
        songQueue.clear()
        songQueue.addAll(arg as SongQueue)
        Handler(appCtx.mainLooper).post(Runnable(this::notifyDataSetChanged))
    }

    /**
     * @return true if event was handled, false otherwise
     */
    fun onBackPressed(): Boolean {
        return when(state) {
            State.SWAP_SELECTION -> {
                setState(State.INITIAL)
                true
            }
            else -> false
        }
    }

    private fun setState(s: State) {
        state = s
        Handler(appCtx.mainLooper).post(this::notifyDataSetChanged)
    }

    override fun getView(position: Int, v: View?, viewGroup: ViewGroup?): View {
        val view = v ?: layoutInflater.inflate(R.layout.server_song, viewGroup, false)
        val song = this.songQueue[position]
        val appState = stateService.getState()
        var userName = preferences.getString("client_name", appCtx.getString(R.string.client_name_default)) as String
        userName = if (userName.isBlank()) appCtx.getString(R.string.client_name_default) else userName

        view.title.text = song.title
        view.title.isSelected = true
        view.author.text = song.authorName
        view.author.isSelected = true
        view.sent_by.text =
            if (song.sentBy == null || !song.sentBy.isEmpty())
                appCtx.getString(R.string.song_queue_item_sent_by, song.sentBy ?: userName)
            else ""
        view.sent_by.isSelected = true
        val duration : String = Misc.formatTime(song.durationMS.toLong())
        view.duration.text = appCtx.getString(R.string.song_item_duration_format, duration)
        view.duration.isSelected = true
        view.sender_ip.text = song.ip ?: view.context.getString(R.string.error_message_no_ip)
        view.sender_ip.isSelected = true
        view.sender_mac.text = song.mac ?: view.context.getString(R.string.error_message_no_mac)
        view.sender_mac.isSelected = true
        view.layout_admin.visibility = if (appState.canDisplaySongOwnerData()) View.VISIBLE else View.INVISIBLE
        view.remove_song.visibility = if (appState.canRemoveSong(song)) View.VISIBLE else View.INVISIBLE
        val isHighlighted =
            appState.isSongHighlighted(song) ||
            (state == State.SWAP_SELECTION && firstSelection?.id == song.id)
        view.setBackgroundResource(
            if (isHighlighted) R.color.highlight
            else android.R.color.background_light
        )

        view.remove_song.setOnClickListener {
            eventMgr.dispatchEvent(DeleteSongEvent(song))
        }
        val gd = GestureDetector(appCtx, object: GestureDetector.SimpleOnGestureListener() {
            override fun onLongPress(e: MotionEvent?) {
                super.onLongPress(e)
                if (stateService.getState().type == AppStateService.State.Admin &&
                    this@SongQueueAdapter.state == State.INITIAL
                ) {
                    firstSelection = this@SongQueueAdapter.songQueue[position]
                    setState(State.SWAP_SELECTION)
                    Log.d("LocalSongListState", state.name)
                }
            }

            override fun onSingleTapConfirmed(e: MotionEvent?): Boolean {
                if (stateService.getState().type == AppStateService.State.Admin &&
                    this@SongQueueAdapter.state == State.SWAP_SELECTION
                ) {
                    val secondSelection = this@SongQueueAdapter.songQueue[position]
                    if (secondSelection.id != firstSelection!!.id) {
                        Log.d(
                            "LocalSongListState",
                            "[${firstSelection!!.title}] && [${secondSelection.title}]")

                        eventMgr.dispatchEvent(
                            SwapSongsRequestEvent(Pair(firstSelection!!, secondSelection)))

                        firstSelection = null
                        setState(State.INITIAL)
                        Log.d("LocalSongListState", state.name)
                        return true
                    }
                }
                return super.onSingleTapConfirmed(e)
            }
        })
        view.setOnTouchListener { _: View?, event: MotionEvent? ->
            gd.onTouchEvent(event)
        }
        return view
    }
    override fun getItemId(p: Int): Long {
        return p.toLong()
    }

    override fun getCount(): Int {
        return this.songQueue.size
    }
    override fun getItem(item: Int): Any {
        return this.songQueue[item]
    }
}
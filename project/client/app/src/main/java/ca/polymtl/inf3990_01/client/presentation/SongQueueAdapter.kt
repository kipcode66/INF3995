package ca.polymtl.inf3990_01.client.presentation

import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.SongQueue
import kotlinx.android.synthetic.main.server_song.view.*

class SongQueueAdapter(val songQueue: SongQueue, val layoutInflater: LayoutInflater, val stateService: AppStateService): BaseAdapter() {
    override fun getView(postion: Int, v: View?, viewGroup: ViewGroup?): View {
        val view = v ?: layoutInflater.inflate(R.layout.server_song, viewGroup, false)
        val song = this.songQueue[postion]

        view.title.text = song.title
        view.author.text = song.authorName
        view.sent_by.text = song.sentBy
        view.sender_ip.text = song.ip ?: view.context.getString(R.string.error_message_no_ip)
        view.sender_mac.text = song.mac ?: view.context.getString(R.string.error_message_no_mac)
        view.layout_admin.visibility = if (stateService.getState().canDisplaySongOwnerData()) View.VISIBLE else View.GONE
        view.remove_song.visibility = if (stateService.getState().canRemoveSong(song)) View.VISIBLE else View.INVISIBLE
        view.remove_song.setOnClickListener {
            //TODO : send query to server to remove 'song'
        }
        return view
    }
    override fun getItemId(p: Int): Long {
        return  p.toLong()
    }

    override fun getCount(): Int {
        return this.songQueue.size
    }
    override fun getItem(item: Int): Any {
        return this.songQueue[item]
    }
}
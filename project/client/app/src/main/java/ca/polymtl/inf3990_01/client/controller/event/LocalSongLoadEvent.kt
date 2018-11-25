package ca.polymtl.inf3990_01.client.controller.event

import android.app.Activity

class LocalSongLoadEvent(val activity: Activity): Event {
    companion object {
        const val EVENT_TYPE = "local_song_load"
    }
    override val type = EVENT_TYPE
}
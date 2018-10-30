package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.model.LocalSong

class SendSongEvent(val song: LocalSong): Event {
    companion object {
        const val EVENT_TYPE = "send_song_event"
    }
    override val type = EVENT_TYPE
}
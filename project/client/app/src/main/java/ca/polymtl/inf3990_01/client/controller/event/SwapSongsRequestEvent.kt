package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.model.Song

class SwapSongsRequestEvent(val songs: Pair<Song, Song>): Event {
    companion object {
        const val EVENT_TYPE = "swap_songs_request_event"
    }
    override val type = EVENT_TYPE
}

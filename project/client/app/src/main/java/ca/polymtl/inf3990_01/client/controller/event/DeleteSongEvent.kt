package ca.polymtl.inf3990_01.client.controller.event

import ca.polymtl.inf3990_01.client.model.Song

class DeleteSongEvent(val song: Song): Event {
    companion object {
        const val EVENT_TYPE = "delete_song"
    }
    override val type = EVENT_TYPE
}
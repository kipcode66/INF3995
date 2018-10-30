package ca.polymtl.inf3990_01.client.controller.event

class LocalSongLoadEvent: Event {
    companion object {
        const val EVENT_TYPE = "local_song_load"
    }
    override val type = EVENT_TYPE
}
package ca.polymtl.inf3990_01.client.controller.rest

import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.Song

class RestRequestService(private val tokenMgr: TokenManagerService, private val httpClient: HTTPRestClient) {
    suspend fun getSongList(): List<Song> {
        TODO("Not Implemented")
    }

    suspend fun sendSong(song: LocalSong) {
        TODO("Not Implemented")
    }

    suspend fun deleteSong(song: Song) {
        TODO("Not Implemented")
    }
}

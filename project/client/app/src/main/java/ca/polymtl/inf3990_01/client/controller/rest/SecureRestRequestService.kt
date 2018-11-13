package ca.polymtl.inf3990_01.client.controller.rest

import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.model.SoundVolume
import ca.polymtl.inf3990_01.client.model.Statistics
import ca.polymtl.inf3990_01.client.model.User

class SecureRestRequestService {
    suspend fun getSongList(): List<Song> {
        TODO("Not Implemented")
    }

    suspend fun deleteSong(song: Song) {
        TODO("Not Implemented")
    }

    suspend fun swapSongs(pair: Pair<Song, Song>) {
        TODO("Not Implemented")
    }

    suspend fun getVolume(): SoundVolume {
        TODO("Not Implemented")
    }

    suspend fun getStatistics(): Statistics {
        TODO("Not Implemented")
    }

    suspend fun blockUser(user: User) {
        TODO("Not Implemented")
    }

    suspend fun unblockUser(user: User) {
        TODO("Not Implemented")
    }

    suspend fun getBlackList(): List<User> {
        TODO("Not Implemented")
    }
}
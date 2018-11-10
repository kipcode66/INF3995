package ca.polymtl.inf3990_01.client.model

import android.net.Uri

open class LocalSong(
        title: String,
        authorName: String,
        durationSec: Int,
        val file: Uri
): Song(title, authorName, durationSec, -1, null), Comparable<LocalSong> {
    override fun compareTo(other: LocalSong): Int =
        when {
            title.compareTo(other.title) != 0             -> title.compareTo(other.title)
            authorName.compareTo(other.authorName) != 0   -> authorName.compareTo(other.authorName)
            durationSec.compareTo(other.durationSec) != 0 -> durationSec.compareTo(other.durationSec)
            else                                          -> file.compareTo(other.file)
        }
}
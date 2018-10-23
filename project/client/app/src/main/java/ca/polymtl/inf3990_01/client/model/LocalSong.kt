package ca.polymtl.inf3990_01.client.model

import android.net.Uri

open class LocalSong(
        title: String,
        authorName: String,
        durationSec: Int,
        val file: Uri
): Song(title, authorName, durationSec, -1, null)
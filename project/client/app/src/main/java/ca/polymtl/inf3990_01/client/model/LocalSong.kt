package ca.polymtl.inf3990_01.client.model

import android.net.Uri

open class LocalSong(title: String, authorName: String, val file: Uri): Song(title, authorName)
package ca.polymtl.inf3990_01.client.controller

import android.app.Activity
import android.content.Context
import android.content.pm.PackageManager
import android.net.Uri
import android.provider.MediaStore
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.LocalSongs


class LocalSongController(
        private val appCtx: Context,
        private val dataProvider: DataProvider
) {
    companion object { private const val CODE_PERMISSIONS = 133 }

    fun reloadLocalSong(activity: Activity) {
        verifyPermissions(activity)
    }

    private fun verifyPermissions(activity: Activity) {
        if (activity.checkSelfPermission(android.Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            activity.requestPermissions(arrayOf(android.Manifest.permission.READ_EXTERNAL_STORAGE), CODE_PERMISSIONS)
            return
        }
        val localSongs = LocalSongs()
        localSongs.addAll(findSong(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI))
        localSongs.addAll(findSong(MediaStore.Audio.Media.INTERNAL_CONTENT_URI))
        dataProvider.setLocalSongs(localSongs)
    }

    private fun findSong(songsURI: Uri): LocalSongs {
        val songsList = LocalSongs()
        val music = MediaStore.Audio.Media.IS_MUSIC + "!=0 and " + MediaStore.Audio.Media.MIME_TYPE + "==\"audio/mpeg\""
        val cursor = appCtx.contentResolver.query(songsURI, null, music, null, null)
        if (cursor != null) {
            if (cursor.moveToFirst()) {
                do {
                    val authorName = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.ARTIST))
                    val songTitle = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.TITLE))
                    val songFileUri = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.DATA))
                    val duration = cursor.getInt(cursor.getColumnIndex(MediaStore.Audio.Media.DURATION))
                    songsList.add(LocalSong(songTitle, authorName, duration, Uri.parse(songFileUri)))
                } while (cursor.moveToNext())
            }
            cursor.close()
        }
        return songsList
    }
}
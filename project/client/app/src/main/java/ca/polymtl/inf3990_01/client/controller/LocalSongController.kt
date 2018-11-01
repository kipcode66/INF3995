package ca.polymtl.inf3990_01.client.controller

import android.content.Context
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Handler
import android.provider.MediaStore
import android.support.v4.app.ActivityCompat
import android.support.v4.app.ActivityCompat.requestPermissions
import android.support.v4.content.ContextCompat
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.LocalSongLoadEvent
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.LocalSongs
import ca.polymtl.inf3990_01.client.presentation.Presenter


class LocalSongController(
        private val appCtx: Context,
        private val eventMgr: EventManager,
        private val presenter: Presenter
) {

    fun reloadLocalSong() {
        verifyPermsions()
    }

    private val CODE_PERMISSIONS = 133

    fun verifyPermsions() {
        if (ActivityCompat.checkSelfPermission(appCtx, android.Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            //requestPermissions(appCtx, arrayOf(android.Manifest.permission.READ_EXTERNAL_STORAGE), CODE_PERMISSIONS)
            return
        }
        val localSongs: LocalSongs = LocalSongs()
        localSongs.addAll(findSong(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI))
        localSongs.addAll(findSong(MediaStore.Audio.Media.INTERNAL_CONTENT_URI))
        presenter.setLocalSongs(localSongs)
    }

    fun findSong(songsURI: Uri): LocalSongs {
        val songsList: LocalSongs = LocalSongs()
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
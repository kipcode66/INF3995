package ca.polymtl.inf3990_01.client.view

import android.content.pm.PackageManager
import android.net.Uri
import android.os.Bundle
import android.provider.MediaStore
import android.support.v4.app.ActivityCompat
import android.view.Menu
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.model.LocalSong
import kotlinx.android.synthetic.main.content_local_song.*
import kotlinx.android.synthetic.main.local_song.view.*

class LocalSongActivity : AbstractDrawerActivity(R.layout.activity_local_song, R.id.drawer_layout) {

    var songsList=ArrayList<LocalSong>()
    var adapter:SongAdapter=SongAdapter(songsList)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        SongsList.adapter = adapter
        verifyPermsions()
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.local_song, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        when (item.itemId) {
            R.id.action_reload -> {
                songsList.clear()
                foundSong(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI)
                foundSong(MediaStore.Audio.Media.INTERNAL_CONTENT_URI)
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }

    inner  class SongAdapter(val listOfSongOnTheDevice: ArrayList<LocalSong>): BaseAdapter() {
        override fun getView(postion: Int, v: View?, viewGroup: ViewGroup?): View {
            val view = v ?: layoutInflater.inflate(R.layout.local_song, viewGroup, false)
            val song = this.listOfSongOnTheDevice[postion]
            view.songName.text = song.title
            view.author.text = song.authorName
            view.send.setOnClickListener {
                //TODO : send local_song to server
            }
            return view

        }
        override fun getItemId(p: Int): Long {
            return  p.toLong()
        }

        override fun getCount(): Int {
            return this.listOfSongOnTheDevice.size
        }
        override fun getItem(item: Int): Any {
            return this.listOfSongOnTheDevice[item]
        }
    }
    private val CODE_PERMISSIONS = 133

    fun verifyPermsions() {
        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.READ_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            requestPermissions(arrayOf(android.Manifest.permission.READ_EXTERNAL_STORAGE),
                    CODE_PERMISSIONS)
            return
        }
        songsList.clear()
        foundSong(MediaStore.Audio.Media.EXTERNAL_CONTENT_URI)
        foundSong(MediaStore.Audio.Media.INTERNAL_CONTENT_URI)
    }
    fun foundSong(songsURI: Uri) {
        val music = MediaStore.Audio.Media.IS_MUSIC + "!=0 and " + MediaStore.Audio.Media.MIME_TYPE + "==\"audio/mpeg\""
        val cursor = contentResolver.query(songsURI, null, music, null, null)
        if (cursor != null) {
            if (cursor.moveToFirst()) {
                do {
                    val authorName = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.ARTIST))
                    val songTitle = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.TITLE))
                    val songFileUri = cursor.getString(cursor.getColumnIndex(MediaStore.Audio.Media.DATA))
                    songsList.add(LocalSong(songTitle, authorName, Uri.parse(songFileUri)))
                } while (cursor.moveToNext())
            }
            cursor.close()
        }
    }
}


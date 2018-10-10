package ca.polymtl.inf3990_01.client.view
import android.content.pm.PackageManager
import android.os.Bundle
import android.provider.MediaStore
import android.support.v4.app.ActivityCompat
import android.support.v7.app.AppCompatActivity
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import kotlinx.android.synthetic.main.activity_local_song.*
import ca.polymtl.inf3990_01.client.R
import kotlinx.android.synthetic.main.song.view.*


class LocalSongActivity : AppCompatActivity(){


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_local_song)
        verifyPermsions()
    }

    var songsList=ArrayList<Song>()
    var adapter:SongAdapter?=null

    inner  class SongAdapter: BaseAdapter {

        var listOfSongOnTheDevice = ArrayList<Song>()
        constructor(listOfSongOnTheDevice: ArrayList<Song>) : super() {
            this.listOfSongOnTheDevice = listOfSongOnTheDevice
        }
        override fun getView(postion: Int, view: View?, viewGroup: ViewGroup?): View {
            val view = layoutInflater.inflate(R.layout.song, null)
            val song = this.listOfSongOnTheDevice[postion]
            view.songName.text = song.title
            view.author.text = song.authorName
            view.send.setOnClickListener(View.OnClickListener {
                //TODO : send song to server
            })
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
        foundSong()

    }
    fun foundSong() {
        val songsURI = MediaStore.Audio.Media.EXTERNAL_CONTENT_URI
        val music = MediaStore.Audio.Media.IS_MUSIC + "!=0"
        val cursor = contentResolver.query(songsURI, null, music, null, null)
        if (cursor != null) {
            if (cursor!!.moveToFirst()) {
                do {
                    val authorName = cursor!!.getString(cursor!!.getColumnIndex(MediaStore.Audio.Media.ARTIST))
                    val songTitle = cursor!!.getString(cursor!!.getColumnIndex(MediaStore.Audio.Media.DISPLAY_NAME))
                    songsList.add(Song(songTitle, authorName))
                } while (cursor!!.moveToNext())
            }
            cursor!!.close()
            adapter = SongAdapter(songsList)
            SongsList.adapter = adapter
        }
    }
}


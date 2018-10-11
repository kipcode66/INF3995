package ca.polymtl.inf3990_01.client.view
import android.content.Intent
import android.content.pm.PackageManager
import android.net.Uri
import android.os.Bundle
import android.provider.MediaStore
import android.support.design.widget.NavigationView
import android.support.v4.app.ActivityCompat
import android.support.v4.view.GravityCompat
import android.support.v7.app.ActionBarDrawerToggle
import android.support.v7.app.AppCompatActivity
import android.view.MenuItem
import android.view.View
import android.view.ViewGroup
import android.widget.BaseAdapter
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.model.LocalSong
import kotlinx.android.synthetic.main.activity_local_song.*
import kotlinx.android.synthetic.main.app_bar_local_song.*
import kotlinx.android.synthetic.main.content_local_song.*
import kotlinx.android.synthetic.main.song.view.*


class LocalSongActivity : AppCompatActivity(), NavigationView.OnNavigationItemSelectedListener {

    var songsList=ArrayList<LocalSong>()
    var adapter:SongAdapter=SongAdapter(songsList)

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_local_song)
        setSupportActionBar(toolbar)

        val toggle = ActionBarDrawerToggle(
                this, drawer_layout, toolbar, R.string.navigation_drawer_open, R.string.navigation_drawer_close)
        drawer_layout.addDrawerListener(toggle)
        toggle.syncState()

        nav_view.setNavigationItemSelectedListener(this)

        SongsList.adapter = adapter
        verifyPermsions()
    }

    override fun onBackPressed() {
        if (drawer_layout.isDrawerOpen(GravityCompat.START)) {
            drawer_layout.closeDrawer(GravityCompat.START)
        } else {
            super.onBackPressed()
        }
    }

    inner  class SongAdapter(val listOfSongOnTheDevice: ArrayList<LocalSong>): BaseAdapter() {
        override fun getView(postion: Int, v: View?, viewGroup: ViewGroup?): View {
            val view = v ?: layoutInflater.inflate(R.layout.song, viewGroup, false)
            val song = this.listOfSongOnTheDevice[postion]
            view.songName.text = song.title
            view.author.text = song.authorName
            view.send.setOnClickListener {
                //TODO : send song to server
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


    override fun onNavigationItemSelected(item: MenuItem): Boolean {
        // Handle navigation view item clicks here.
        when (item.itemId) {
            R.id.nav_queue -> {
                val intent = Intent(this, QueueActivity::class.java)
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT or Intent.FLAG_ACTIVITY_LAUNCHED_FROM_HISTORY)
                startActivity(intent)
            }
            R.id.nav_local_song -> {
                val intent = Intent(this, LocalSongActivity::class.java)
                intent.addFlags(Intent.FLAG_ACTIVITY_REORDER_TO_FRONT)
                startActivity(intent)
            }
            R.id.nav_slideshow -> {

            }
            R.id.nav_manage -> {

            }
            R.id.nav_share -> {

            }
            R.id.nav_send -> {

            }
        }

        drawer_layout.closeDrawer(GravityCompat.START)
        return true
    }
}


package ca.polymtl.inf3990_01.client.view
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.LocalSongLoadEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.LocalSongs
import kotlinx.android.synthetic.main.content_local_song.*
import ca.polymtl.inf3990_01.client.presentation.LocalSongAdapter
import org.koin.android.ext.android.inject
import org.koin.core.parameter.ParameterList

class LocalSongActivity : AbstractDrawerActivity(R.layout.activity_local_song, R.id.drawer_layout) {

    var songsList = LocalSongs()
    val eventMgr: EventManager by inject()
    val localSongAdapter: LocalSongAdapter by inject{ ParameterList(songsList, layoutInflater) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        songs_list.adapter = localSongAdapter
        eventMgr.dispatchEvent(LocalSongLoadEvent())
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
            R.id.action_change_state -> {
                val currState = stateService.getState().type
                val v = AppStateService.State.values()
                stateService.setState(v[(v.indexOf(currState) + 1) % v.size])
                return true
            }
            R.id.action_reload -> {
                songsList.clear()
                eventMgr.dispatchEvent(LocalSongLoadEvent())
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }
}

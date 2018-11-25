package ca.polymtl.inf3990_01.client.view
import android.os.Bundle
import android.os.Handler
import android.view.Menu
import android.view.MenuItem
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.LocalSongLoadEvent
import ca.polymtl.inf3990_01.client.controller.event.LogoutRequestEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.LocalSongs
import kotlinx.android.synthetic.main.content_local_song.*
import ca.polymtl.inf3990_01.client.presentation.LocalSongAdapter
import org.koin.android.ext.android.inject
import org.koin.core.parameter.ParameterList
import java.util.*

class LocalSongActivity : AbstractDrawerActivity(R.layout.activity_local_song, R.id.drawer_layout) {

    private var songsList = LocalSongs()
    private val eventMgr: EventManager by inject()
    private val appStateService: AppStateService by inject()
    private val localSongAdapter: LocalSongAdapter by inject{ ParameterList(songsList, layoutInflater) }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStateChange(o: Observable?, arg: Any?) {
        Handler(this.mainLooper).post(this::invalidateOptionsMenu)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        songs_list.adapter = localSongAdapter
        eventMgr.dispatchEvent(LocalSongLoadEvent(this))
        appStateService.addObserver(Observer(this::onAppStateChange))
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.local_song, menu)
        menu.findItem(R.id.action_show_login).isVisible = appStateService.getState().type == AppStateService.State.User
        menu.findItem(R.id.action_disconnect).isVisible = appStateService.getState().type == AppStateService.State.Admin
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        when (item.itemId) {
            R.id.action_reload -> {
                songsList.clear()
                eventMgr.dispatchEvent(LocalSongLoadEvent(this))
                return true
            }
            R.id.action_show_login -> {
                val loginDialog = LoginDialog(this, eventMgr)
                loginDialog.show()
                return true
            }
            R.id.action_disconnect -> {
                eventMgr.dispatchEvent(LogoutRequestEvent())
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }
}


package ca.polymtl.inf3990_01.client.view

import android.os.Bundle
import android.os.Handler
import android.view.Menu
import android.view.MenuItem
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.LogoutRequestEvent
import ca.polymtl.inf3990_01.client.controller.event.RequestQueueReloadEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.DataProvider
import ca.polymtl.inf3990_01.client.model.SongQueue
import ca.polymtl.inf3990_01.client.presentation.SongQueueAdapter
import kotlinx.android.synthetic.main.content_queue.*
import org.koin.android.ext.android.inject
import org.koin.core.parameter.ParameterList
import java.util.*

import kotlinx.coroutines.experimental.javafx.JavaFx as Main

class QueueActivity : AbstractDrawerActivity(R.layout.activity_queue, R.id.drawer_layout) {

    private val eventMgr: EventManager by inject()
    private val appStateService: AppStateService by inject()
    private val dataProvider: DataProvider by inject()

    private val songQueue: SongQueue = SongQueue()
    private val songQueueAdapter: SongQueueAdapter by inject{ ParameterList(songQueue, layoutInflater) }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStateChange(o: Observable?, arg: Any?) {
        Handler(this.mainLooper).post(this::invalidateOptionsMenu)
    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        appStateService.addObserver(Observer(this::onAppStateChange))
        song_queue.adapter = songQueueAdapter
        content_queue_swipe.setColorSchemeResources(R.color.colorPrimary, R.color.colorPrimaryDark)
        content_queue_swipe.setOnRefreshListener { eventMgr.dispatchEvent(RequestQueueReloadEvent()) }
        dataProvider.observeSongQueue(Observer(this::onSongQueueChange))
    }

    private fun refresh() {
        Handler(mainLooper).post {content_queue_swipe.isRefreshing = true}
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onSongQueueChange(o: Observable, arg: Any?) {
        Handler(mainLooper).post {content_queue_swipe.isRefreshing = false}
    }

    override fun onBackPressed() {
        if (!songQueueAdapter.onBackPressed()) {
            super.onBackPressed()
        }
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.queue, menu)
        menu.findItem(R.id.action_show_login).isVisible = appStateService.getState().type == AppStateService.State.User
        menu.findItem(R.id.action_disconnect).isVisible = appStateService.getState().type == AppStateService.State.Admin
        menu.findItem(R.id.action_block_user).isVisible = appStateService.getState().type == AppStateService.State.Admin
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        when (item.itemId) {
            R.id.action_reload -> {
                refresh()
                return true
            }
            R.id.action_block_user -> {
                BlockUserDialog(this, eventMgr).show()
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

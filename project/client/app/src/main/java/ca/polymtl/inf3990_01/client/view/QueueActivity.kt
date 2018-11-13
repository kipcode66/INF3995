package ca.polymtl.inf3990_01.client.view

import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.RequestQueueReloadEvent
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.SongQueue
import ca.polymtl.inf3990_01.client.presentation.SongQueueAdapter
import kotlinx.android.synthetic.main.content_queue.*
import org.koin.android.ext.android.inject
import org.koin.core.parameter.ParameterList

import kotlinx.coroutines.experimental.javafx.JavaFx as Main

class QueueActivity : AbstractDrawerActivity(R.layout.activity_queue, R.id.drawer_layout) {

    val eventMgr: EventManager by inject()

    val songQueue: SongQueue = SongQueue()
    val songQueueAdapter: SongQueueAdapter by inject{ ParameterList(songQueue, layoutInflater) }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        song_queue.adapter = songQueueAdapter
    }

    override fun onCreateOptionsMenu(menu: Menu): Boolean {
        // Inflate the menu; this adds items to the action bar if it is present.
        menuInflater.inflate(R.menu.queue, menu)
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
                eventMgr.dispatchEvent(RequestQueueReloadEvent())
                return true
            }
            else -> return super.onOptionsItemSelected(item)
        }
    }
}

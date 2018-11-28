package ca.polymtl.inf3990_01.client.controller.state

import android.content.Context
import android.os.Handler
import android.view.Menu
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.view.AbstractDrawerActivity
import ca.polymtl.inf3990_01.client.view.BlackListActivity
import ca.polymtl.inf3990_01.client.view.StatisticsActivity
import java.util.*

class AppStateService(private val appCtx: Context): Observable() {
    enum class State {
        User,
        Admin;
    }
    private val states: Map<State, AppState> = mapOf(
            Pair(State.User, object : AppState {
                override val type = State.User

                override fun updateNavigationView(menu: Menu) {
                    menu.findItem(R.id.nav_blacklist).isVisible = false
                    menu.findItem(R.id.nav_statistics).isVisible = false
                    menu.removeItem(menu.add("").itemId) // Forces an update of the view
                }

                override fun isSongHighlighted(song: Song): Boolean = song.sentBy == null

                override fun canRemoveSong(song: Song, position: Int): Boolean {
                    return song.sentBy == null && position > 0
                }

                override fun canDisplaySongOwnerData(): Boolean = false
                override fun <A: AbstractDrawerActivity> finishActivityIfNeeded(activity: A) {
                    when (activity.javaClass) {
                        BlackListActivity::class.java,
                        StatisticsActivity::class.java -> activity.finish()
                    }
                }
            }),
            Pair(State.Admin, object : AppState {
                override val type = State.Admin

                override fun updateNavigationView(menu: Menu) {
                    menu.findItem(R.id.nav_blacklist).isVisible = true
                    menu.findItem(R.id.nav_statistics).isVisible = true
                    menu.removeItem(menu.add("").itemId) // Forces an update of the view
                }

                override fun isSongHighlighted(song: Song): Boolean = false

                override fun canRemoveSong(song: Song, position: Int): Boolean = true

                override fun canDisplaySongOwnerData(): Boolean = true
                override fun <A: AbstractDrawerActivity> finishActivityIfNeeded(activity: A) {}
            }))

    private var stateInternal: AppState = states[State.User]!!

    init {
        setState(State.User)
    }

    fun getState(): AppState {
        return stateInternal
    }

    fun setState(state: State) {
        stateInternal = states[state]!!
        setChanged()
        Handler(appCtx.mainLooper).post(this::notifyObservers)
    }
}
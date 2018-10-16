package ca.polymtl.inf3990_01.client.controller.state

import android.support.design.widget.NavigationView
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.view.AbstractDrawerActivity
import ca.polymtl.inf3990_01.client.view.BlackListActivity
import ca.polymtl.inf3990_01.client.view.StatisticsActivity
import java.util.*

class AppStateService: Observable() {
    enum class State {
        User,
        Admin;
    }
    private val states: Map<State, AppState> = mapOf(
            Pair(State.User, object : AppState {
                override val type = State.User

                override fun updateNavigationView(navView: NavigationView) {
                    navView.menu.findItem(R.id.nav_blacklist).isVisible = false
                    navView.menu.findItem(R.id.nav_statistics).isVisible = false
                }

                override fun sendFile(song: LocalSong): Boolean {
                    TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
                }

                override fun canRemoveSong(song: Song): Boolean {
                    return song.sentBy == null
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

                override fun updateNavigationView(navView: NavigationView) {
                    navView.menu.findItem(R.id.nav_blacklist).isVisible = true
                    navView.menu.findItem(R.id.nav_statistics).isVisible = true
                }

                override fun sendFile(song: LocalSong): Boolean {
                    TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
                }

                override fun canRemoveSong(song: Song): Boolean = true

                override fun canDisplaySongOwnerData(): Boolean = true
                override fun <A: AbstractDrawerActivity> finishActivityIfNeeded(activity: A) {}
            }))

    private var mStateInternal: AppState = states[State.User]!!

    init {
        setState(State.User)
    }

    fun getState(): AppState {
        return mStateInternal
    }

    fun setState(state: State) {
        mStateInternal = states[state]!!
        setChanged()
        notifyObservers()
    }
}
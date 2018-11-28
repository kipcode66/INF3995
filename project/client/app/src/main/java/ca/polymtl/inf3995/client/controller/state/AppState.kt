package ca.polymtl.inf3995.client.controller.state

import android.view.Menu
import ca.polymtl.inf3995.client.model.Song
import ca.polymtl.inf3995.client.view.AbstractDrawerActivity

interface AppState {
    val type: AppStateService.State
    fun updateNavigationView(menu: Menu)
    fun isSongHighlighted(song: Song): Boolean
    fun canRemoveSong(song: Song, position: Int): Boolean
    fun canDisplaySongOwnerData(): Boolean
    fun <A: AbstractDrawerActivity> finishActivityIfNeeded(activity: A)
}
package ca.polymtl.inf3990_01.client.controller.state

import android.support.design.widget.NavigationView
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.view.AbstractDrawerActivity

interface AppState {
    val type: AppStateService.State
    fun updateNavigationView(navView: NavigationView)
    fun isSongHighlighted(song: Song): Boolean
    fun sendFile(song: LocalSong): Boolean
    fun canRemoveSong(song: Song): Boolean
    fun canDisplaySongOwnerData(): Boolean
    fun <A: AbstractDrawerActivity> finishActivityIfNeeded(activity: A)
}
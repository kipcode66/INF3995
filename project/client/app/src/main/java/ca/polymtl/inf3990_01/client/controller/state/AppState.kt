package ca.polymtl.inf3990_01.client.controller.state

import android.support.design.widget.NavigationView
import ca.polymtl.inf3990_01.client.model.LocalSong

interface AppState {
    val type: AppStateService.State
    fun updateNavigationView(navView: NavigationView)
    fun sendFile(song: LocalSong): Boolean
}
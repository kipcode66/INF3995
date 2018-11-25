package ca.polymtl.inf3990_01.client.controller

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.media.AudioManager
import android.support.v4.media.VolumeProviderCompat
import android.support.v4.media.session.MediaSessionCompat
import android.support.v4.media.session.MediaSessionCompat.FLAG_HANDLES_MEDIA_BUTTONS
import android.support.v4.media.session.PlaybackStateCompat
import ca.polymtl.inf3990_01.client.controller.event.*
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.Volume
import java.util.*


class VolumeController(
    private val appCtx: Context,
    private val eventMgr: EventManager,
    private val stateService: AppStateService
): BroadcastReceiver() {
    companion object {
        const val PERCENTAGE_MAX = 100
        const val PERCENTAGE_INITIAL = 0
        const val PERCENTAGE_INCREMENT = 5
    }

    override fun onReceive(context: Context?, intent: Intent?) {
    }

    private val mediaSession = MediaSessionCompat(appCtx, javaClass.name)
    private val volumeProvider =
        object : VolumeProviderCompat(VolumeProviderCompat.VOLUME_CONTROL_RELATIVE, PERCENTAGE_MAX, PERCENTAGE_INITIAL) {
            fun setVolumeLevel(volume: Volume) {
                currentVolume = if (volume.mute) 0 else volume.level
            }

            override fun onSetVolumeTo(volume: Int) {
                super.onSetVolumeTo(volume)
                val diff = volume - currentVolume
                when {
                    diff > 0 -> eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.INCREASE, diff))
                    diff < 0 -> eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.DECREASE, -diff))
                }
            }

            override fun onAdjustVolume(direction: Int) {
                super.onAdjustVolume(direction)
                when (direction) {
                    AudioManager.ADJUST_RAISE -> {
                        eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.INCREASE, PERCENTAGE_INCREMENT))
                    }
                    AudioManager.ADJUST_LOWER -> {
                        eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.DECREASE, PERCENTAGE_INCREMENT))
                    }
                    AudioManager.ADJUST_MUTE -> {
                        eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.MUTE))
                    }
                    AudioManager.ADJUST_UNMUTE -> {
                        eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.UNMUTE))
                    }
                }
            }
    }

    init {
        mediaSession.setFlags(FLAG_HANDLES_MEDIA_BUTTONS)
        mediaSession.setPlaybackToRemote(volumeProvider)
        val state = PlaybackStateCompat.Builder()
        .setActions(0)
        .setState(PlaybackStateCompat.STATE_PLAYING, 0, 1.0f)
        .build()
        mediaSession.setPlaybackState(state)
        eventMgr.addEventListener(this::onVolumeChanged)
        eventMgr.addEventListener(this::onAppTerminate)
        eventMgr.addEventListener(this::onAppResume)
        eventMgr.addEventListener(this::onAppStop)
        stateService.addObserver(this::onAppStateChange)
    }

    fun getVolume(): Volume {
        return Volume(volumeProvider.currentVolume, volumeProvider.currentVolume <= 0)
    }

    @SuppressWarnings("UNUSED_PARAMETER")
    private fun onAppStateChange(o: Observable, arg: Any?) {
        mediaSession.isActive = stateService.getState().type == AppStateService.State.Admin
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppResume(event: AppResumeEvent) {
        mediaSession.isActive = stateService.getState().type == AppStateService.State.Admin
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppStop(event: AppStopEvent) {
        mediaSession.isActive = false
    }

    private fun onVolumeChanged(event: VolumeChangedEvent) {
        volumeProvider.setVolumeLevel(event.volume)
    }

    @Suppress("UNUSED_PARAMETER")
    private fun onAppTerminate(event: AppTerminateEvent) {
        mediaSession.release()
    }
}
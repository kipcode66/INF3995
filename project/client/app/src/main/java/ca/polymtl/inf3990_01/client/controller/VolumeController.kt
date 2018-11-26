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
        const val PERCENTAGE_MULTIPLIER = 5
    }

    override fun onReceive(context: Context?, intent: Intent?) {
    }

    private val mediaSession = MediaSessionCompat(appCtx, javaClass.name)
    private val volumeProvider =
        object : VolumeProviderCompat(VolumeProviderCompat.VOLUME_CONTROL_ABSOLUTE, PERCENTAGE_MAX / PERCENTAGE_MULTIPLIER, PERCENTAGE_INITIAL / PERCENTAGE_MULTIPLIER) {
            fun setVolumeLevel(volume: Volume) {
                currentVolume = ((volume.level.toDouble() + 1.0) / PERCENTAGE_MULTIPLIER.toDouble()).toInt()
            }

            override fun onSetVolumeTo(volume: Int) {
                super.onSetVolumeTo(volume)
                currentVolume = Math.max(0, Math.min(volume, PERCENTAGE_MAX / PERCENTAGE_MULTIPLIER))
                eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.SET, volume * PERCENTAGE_MULTIPLIER))
            }

            override fun onAdjustVolume(direction: Int) {
                super.onAdjustVolume(direction)
                when (direction) {
                    AudioManager.ADJUST_RAISE  -> {
                        ++currentVolume
                        currentVolume = Math.max(0, Math.min(currentVolume, PERCENTAGE_MAX / PERCENTAGE_MULTIPLIER))
                        eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.SET, currentVolume * PERCENTAGE_MULTIPLIER))
                    }
                    AudioManager.ADJUST_LOWER  -> {
                        --currentVolume
                        currentVolume = Math.max(0, Math.min(currentVolume, PERCENTAGE_MAX / PERCENTAGE_MULTIPLIER))
                        eventMgr.dispatchEvent(VolumeChangeRequestEvent(VolumeChangeRequestEvent.Companion.Change.SET, currentVolume * PERCENTAGE_MULTIPLIER))
                    }
                    AudioManager.ADJUST_MUTE   -> {
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
package ca.polymtl.inf3990_01.client.controller

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.media.AudioManager
import android.support.v4.media.VolumeProviderCompat
import android.support.v4.media.session.MediaControllerCompat
import android.support.v4.media.session.MediaSessionCompat
import android.support.v4.media.session.MediaSessionCompat.FLAG_HANDLES_MEDIA_BUTTONS
import ca.polymtl.inf3990_01.client.controller.event.AppTerminateEvent
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.VolumeChangedEvent
import ca.polymtl.inf3990_01.client.controller.rest.SecureRestRequestService
import ca.polymtl.inf3990_01.client.model.Volume
import kotlinx.coroutines.experimental.launch

class VolumeController(
    private val appCtx: Context,
    private val eventMgr: EventManager,
    private val secureRestService: SecureRestRequestService
): BroadcastReceiver() {
    companion object {
        const val PERCENTAGE_MAX = 100
        const val PERCENTAGE_INITIAL = 0
        const val PERCENTAGE_INCREMENT = 5
    }

    override fun onReceive(context: Context?, intent: Intent?) {
    }

    private val mediaSession = MediaSessionCompat(
        appCtx, "VolumeControllerSession")
    private lateinit var mediaController: MediaControllerCompat
    private val volumeProvider =
        object : VolumeProviderCompat(VolumeProviderCompat.VOLUME_CONTROL_RELATIVE, PERCENTAGE_MAX, PERCENTAGE_INITIAL) {
            fun setVolumeLevel(volume: Volume) {
                currentVolume = if (volume.mute) 0 else volume.level
            }

            override fun onAdjustVolume(direction: Int) {
                super.onAdjustVolume(direction)
                when (direction) {
                    AudioManager.ADJUST_RAISE -> {
                        launch {secureRestService.increaseVolume(PERCENTAGE_INCREMENT)}
                    }
                    AudioManager.ADJUST_LOWER -> {
                        launch {secureRestService.decreaseVolume(PERCENTAGE_INCREMENT)}
                    }
                    AudioManager.ADJUST_MUTE -> {
                        launch {secureRestService.muteVolume()}
                    }
                    AudioManager.ADJUST_UNMUTE -> {
                        launch {secureRestService.unmuteVolume()}
                    }
                }
            }
    }

    init {
        mediaSession.setFlags(FLAG_HANDLES_MEDIA_BUTTONS)
        mediaSession.setPlaybackToRemote(volumeProvider)
        mediaController = MediaControllerCompat(appCtx, mediaSession)
        eventMgr.addEventListener(this::onVolumeChanged)
        eventMgr.addEventListener(this::onAppTerminate)
        mediaSession.isActive = true
    }

    fun getVolume(): Volume {
        return Volume(volumeProvider.currentVolume, volumeProvider.currentVolume <= 0)
    }

    private fun onVolumeChanged(event: VolumeChangedEvent) {
        volumeProvider.setVolumeLevel(event.volume)
    }

    private fun onAppTerminate(event: AppTerminateEvent) {
        mediaSession.release()
    }
}
package ca.polymtl.inf3990_01.client.utils

import java.util.*
import java.util.concurrent.TimeUnit

class Misc {
    companion object {
        fun toObserver(onUpdate: (Observable, Any?) -> Unit): Observer {
            return Observer(onUpdate)
        }

        fun formatTime(milliSeconds: Long): String {
            val hours = TimeUnit.MILLISECONDS.toHours(milliSeconds).toInt() % 24
            val minutes = TimeUnit.MILLISECONDS.toMinutes(milliSeconds).toInt() % 60
            val seconds = TimeUnit.MILLISECONDS.toSeconds(milliSeconds).toInt() % 60
            return when {
                hours > 0 -> String.format("%d:%02d:%02d", hours, minutes, seconds)
                minutes > 0 -> String.format("%02d:%02d", minutes, seconds)
                seconds > 0 -> String.format("00:%02d", seconds)
                else -> {
                    "00:00"
                }
            }
        }

    }
}
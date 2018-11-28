package ca.polymtl.inf3995.client.utils

import java.util.concurrent.TimeUnit

class Misc {
    companion object {
        val IP_REGEX = Regex("^(?:(?:2(?:[0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9])\\.){3}(?:(?:2([0-4][0-9]|5[0-5])|[0-1]?[0-9]?[0-9]))$", setOf(RegexOption.MULTILINE, RegexOption.IGNORE_CASE))
        val MAC_REGEX = Regex("^(?:[a-fA-F0-9]{2}:){5}[a-fA-F0-9]{2}$", setOf(RegexOption.MULTILINE, RegexOption.IGNORE_CASE))

        fun formatTime(milliSeconds: Long): String {
            val hours = TimeUnit.MILLISECONDS.toHours(milliSeconds).toInt() % 24
            val minutes = TimeUnit.MILLISECONDS.toMinutes(milliSeconds).toInt() % 60
            val seconds = TimeUnit.MILLISECONDS.toSeconds(milliSeconds).toInt() % 60
            return when {
                hours > 0   -> String.format("%d:%02d:%02d", hours, minutes, seconds)
                minutes > 0 -> String.format("%02d:%02d", minutes, seconds)
                seconds > 0 -> String.format("00:%02d", seconds)
                else        -> {
                    "00:00"
                }
            }
        }

    }
}
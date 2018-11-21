package ca.polymtl.inf3990_01.client.utils

import java.net.NetworkInterface
import java.nio.ByteBuffer
import java.util.*
import java.util.concurrent.TimeUnit

object NetUtils {
    fun translateIP(ip: Int): String {
        val bytes = ByteBuffer.allocate(4).putInt(ip).array()
                .map(Byte::toPositiveInt)
                .map(Int::toString).reversed()
        return bytes.joinToString(".")
    }

    fun translateIP(ip: String): Int {
        val bytes = ip.split(".").reversed()
                .map(String::toInt)
                .map(Int::toByte).toByteArray()
        return ByteBuffer.wrap(bytes).int
    }

    fun getMacAddress(interfaceName: String): String {
        try {
            val interfaces = Collections.list(NetworkInterface.getNetworkInterfaces())
            for (inter in interfaces) {
                if (!inter.name.equals(interfaceName, ignoreCase = true)) continue
                val mac = inter.hardwareAddress ?: return ""
                val buf = StringBuilder()
                for (aMac in mac) buf.append(String.format("%02x:", aMac))
                if (!buf.isEmpty()) buf.deleteCharAt(buf.length - 1)
                return buf.toString()
            }
        } catch (ignored: Exception) {
        }
        // for now eat exceptions
        return ""
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

fun Byte.toPositiveInt() = toInt() and 0xFF
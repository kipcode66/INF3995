package ca.polymtl.inf3990_01.client.utils

import android.content.Context
import ca.polymtl.inf3990_01.client.model.LocalSong
import java.nio.ByteBuffer
import java.net.NetworkInterface
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
            for (intf in interfaces) {
                if (!intf.name.equals(interfaceName, ignoreCase = true)) continue
                val mac = intf.getHardwareAddress() ?: return ""
                val buf = StringBuilder()
                for (aMac in mac) buf.append(String.format("%02x:", aMac))
                if (buf.length > 0) buf.deleteCharAt(buf.length - 1)
                return buf.toString()
            }
        } catch (ignored: Exception) {
        }
        // for now eat exceptions
        return ""
    }

    fun formatTime(miliSeconds: Long): String {
        val hours = TimeUnit.MILLISECONDS.toHours(miliSeconds).toInt() % 24
        val minutes = TimeUnit.MILLISECONDS.toMinutes(miliSeconds).toInt() % 60
        val seconds = TimeUnit.MILLISECONDS.toSeconds(miliSeconds).toInt() % 60
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
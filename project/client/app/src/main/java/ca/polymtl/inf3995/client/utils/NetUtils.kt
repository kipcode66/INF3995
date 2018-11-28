package ca.polymtl.inf3995.client.utils

import android.util.Base64
import android.util.Base64OutputStream
import ca.polymtl.inf3995.client.model.LocalSong
import java.io.File
import java.io.PipedInputStream
import java.io.PipedOutputStream
import java.net.NetworkInterface
import java.nio.ByteBuffer
import java.util.*

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
                return buf.toString().toUpperCase()
            }
        } catch (ignored: Exception) {
        }
        // for now eat exceptions
        return ""
    }

    fun encoder(song: LocalSong): PipedInputStream {
        val inStream = File(song.file.toString()).inputStream()
        val pin = PipedInputStream()
        val pout = PipedOutputStream(pin)
        val encoder = Base64OutputStream(pout, Base64.NO_WRAP)
        val t = Thread {
            inStream.copyTo(encoder)
            inStream.close()
            encoder.close()
        }
        t.name = "Base64 Encode [${song.file.lastPathSegment}]"
        t.start()
        return pin
    }

}

fun Byte.toPositiveInt() = toInt() and 0xFF
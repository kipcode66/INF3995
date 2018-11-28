package ca.polymtl.inf3995.client.controller.rest

import android.content.Context
import android.content.SharedPreferences
import ca.polymtl.inf3995.client.R
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.HurlStack
import com.android.volley.toolbox.Volley
import java.security.KeyStore
import java.security.cert.CertificateFactory
import javax.net.ssl.SSLContext
import javax.net.ssl.SSLSocketFactory
import javax.net.ssl.TrustManagerFactory

/**
 * It's an HTTPS client to which we can pass a RESTRequest object.
 */
class HTTPSRestClient(private val preferences: SharedPreferences, private val context: Context) {
    private val requestQueue: RequestQueue by lazy {
        // applicationContext is key, it keeps you from leaking the
        // Activity or BroadcastReceiver if someone passes one in.
        Volley.newRequestQueue(context.applicationContext, HurlStack(null, newSslSocketFactory()))
    }

    fun <T> addToRequestQueue(req: Request<T>) {
        requestQueue.add(req)
    }

    fun getBaseURL(): String {
        val serverUrl = preferences.getString("server_url", null)
        return "https://$serverUrl"
    }

    private fun newSslSocketFactory(): SSLSocketFactory {
        try {
            // Get an instance of the Bouncy Castle KeyStore format
            val trusted = KeyStore.getInstance("BKS")
            trusted.load(null, null)
            // Get the raw resource, which contains the keystore with
            // your trusted certificates (root and any intermediate certs)
            val `in` = context.applicationContext.resources.openRawResource(R.raw.ca)
            `in`.use { inStream ->
                val cert = CertificateFactory.getInstance("X.509").generateCertificate(inStream)
                // Initialize the keystore with the provided trusted certificates
                // Provide the password of the keystore
                trusted.setCertificateEntry("elevation_server", cert)
            }

            val tmfAlgorithm = TrustManagerFactory.getDefaultAlgorithm()
            val tmf = TrustManagerFactory.getInstance(tmfAlgorithm)
            tmf.init(trusted)

            val ctx = SSLContext.getInstance("TLS")
            ctx.init(null, tmf.trustManagers, null)

            return ctx.socketFactory
        } catch (e: Exception) {
            throw AssertionError(e)
        }
    }
}
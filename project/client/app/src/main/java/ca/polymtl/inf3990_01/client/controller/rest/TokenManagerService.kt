package ca.polymtl.inf3990_01.client.controller.rest

import android.app.AlertDialog
import android.app.Notification
import android.content.Context
import android.content.SharedPreferences
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import com.android.volley.Request
import com.android.volley.Response
import com.google.gson.Gson
import kotlinx.coroutines.experimental.async
import kotlin.coroutines.experimental.suspendCoroutine
import android.net.wifi.WifiManager
import android.os.Handler
import android.os.Parcel
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.utils.NetUtils
import com.android.volley.DefaultRetryPolicy
import kotlinx.coroutines.experimental.launch
import kotlinx.coroutines.experimental.runBlocking
import java.lang.Exception
import java.lang.RuntimeException


class TokenManagerService(private val appCtx: Context, private val httpClient: HTTPRestClient, private val preferences: SharedPreferences) {
    companion object { // Static properties
        private class GetTokenRequestData(val ip: String, val MAC: String, val nom: String)
        private class GetTokenResponseData(val identificateur: Int, val message: String)

        val SOCKET_TIMEOUT_MS = 3 * 1000
    }

    private var token: Int = 0

    init {
        async {
            getToken()
        }
    }

    suspend fun getToken(): Int {
        // TODO Ensure that the token is still valid.
        val resp = updateToken()
        token = resp.identificateur
        if (resp.identificateur == 0) {
            // TODO Send a signal to the Presenter to show popup with the response message.
            // Temporarly, opening a popup
            Handler(appCtx.mainLooper).post {
                Toast.makeText(appCtx, resp.message, Toast.LENGTH_LONG).show()
            }
        }
        return token
    }

    private suspend fun updateToken(): GetTokenResponseData {
        return suspendCoroutine { continuation ->
            launch {
                var resp: GetTokenResponseData
                try {
                    resp = fetchToken()
                    token = resp.identificateur
                } catch (e: Exception) {
                    // TODO It's here that we put the messages for the errors
                    val msg = appCtx.getString(R.string.network_error)
                    resp = GetTokenResponseData(0, "$msg: ${e.localizedMessage}")
                }
                continuation.resume(resp)
            }
        }
    }

    private suspend fun fetchToken(): GetTokenResponseData {
        val manager = appCtx.getSystemService(Context.WIFI_SERVICE) as WifiManager?
        if (manager != null && manager.isWifiEnabled) {
            val info = manager.connectionInfo
            val ip = NetUtils.translateIP(info.ipAddress)
            val mac = NetUtils.getMacAddress("wlan0")
            return suspendCoroutine { continuation ->
                val request = RESTRequest(
                        Request.Method.GET,
                        httpClient.getBaseURL() + "/usager/identification",
                        Gson().toJson(GetTokenRequestData(
                                ip,
                                mac,
                                preferences.getString("client_name", "")!!
                        )),
                        GetTokenResponseData::class.java,
                        mutableMapOf("X-Auth-Token" to token.toString()), // Headers
                        Response.Listener { respData: ResponseData<GetTokenResponseData> ->
                            continuation.resume(respData.value)
                        },
                        Response.ErrorListener { error ->
                            error.printStackTrace()
                            continuation.resumeWithException(error)
                        }
                )
                request.retryPolicy = DefaultRetryPolicy(
                        SOCKET_TIMEOUT_MS,
                        DefaultRetryPolicy.DEFAULT_MAX_RETRIES,
                        DefaultRetryPolicy.DEFAULT_BACKOFF_MULT
                )
                httpClient.addToRequestQueue(request)
            }
        }
        else {
            return suspendCoroutine { continuation ->
                continuation.resumeWithException(RuntimeException(appCtx.getString(R.string.wifi_disabled)))
            }
        }
    }
}
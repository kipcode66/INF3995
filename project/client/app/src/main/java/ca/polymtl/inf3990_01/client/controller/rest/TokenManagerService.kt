package ca.polymtl.inf3990_01.client.controller.rest

import android.annotation.SuppressLint
import android.content.Context
import android.content.SharedPreferences
import android.net.wifi.WifiManager
import android.os.Handler
import android.util.Log
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import ca.polymtl.inf3990_01.client.utils.NetUtils
import ca.polymtl.inf3990_01.client.utils.SingletonHolder
import com.android.volley.DefaultRetryPolicy
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.VolleyError
import com.google.gson.Gson
import kotlinx.coroutines.experimental.launch
import kotlin.coroutines.experimental.suspendCoroutine


class TokenManagerService private constructor(private val appCtx: Context, private val httpClient: HTTPRestClient, private val preferences: SharedPreferences) {
    companion object { // Static properties
        private class GetTokenRequestData(val ip: String, val MAC: String, val nom: String)
        private class GetTokenResponseData(val identificateur: Int, val message: String)

        const val SOCKET_TIMEOUT_MS = 3 * 1000
        const val RESOURCE_URI = "/usager/identification"
        const val PREFERENCE_KEY_USERNAME = "client_name"
        const val HTTP_HEADER_NAME_X_AUTH_TOKEN = "X-Auth-Token"


        @SuppressLint("StaticFieldLeak")
        @Volatile private var instance: TokenManagerService? = null

        fun getInstance(appCtx: Context, httpClient: HTTPRestClient, preferences: SharedPreferences): TokenManagerService {
            val i = instance
            if (i != null) {
                return i
            }

            return synchronized(this) {
                val i2 = instance
                if (i2 != null) {
                    i2
                } else {
                    val created = TokenManagerService(appCtx, httpClient, preferences)
                    instance = created
                    created
                }
            }
        }

        fun getInstanceOrThrow(): TokenManagerService {
            return synchronized(this) {
                instance!!
            }
        }

        fun hasInstance(): Boolean {
            return synchronized(this) {
                instance != null
            }
        }
    }

    private var token: Int = 0
    private var tokenLock = Object()
    private var lastMessage: String? = null
    private var isUpdating = false

    fun getToken(): Int {
        return token
    }

    suspend fun updateToken() {
        synchronized(isUpdating) {
            if (isUpdating) {
                return
            }
            isUpdating = true
        }

        Log.d(TokenManagerService::class.java.simpleName, "Starting token update")

        var oldToken = token // Dummy value to make Kotlin happy
        synchronized(tokenLock) {
            oldToken = token
        }
        val resp = fetchTokenAndWrapErrors()
        synchronized(tokenLock) {
            token = resp.identificateur
        }
        if ((oldToken != token || resp.identificateur == 0) && lastMessage != resp.message) {
            // TODO Send a signal to the Presenter to show popup with the response message.
            // Temporarly, opening a Toast (a little message at the bottom of the screen)
            Handler(appCtx.mainLooper).post {
                Toast.makeText(appCtx, resp.message, Toast.LENGTH_LONG).show()
                lastMessage = resp.message // Prevent to continuously show the same message (can be annoying)
            }
        }
        Log.d(TokenManagerService::class.java.simpleName, "Token updated")

        synchronized(isUpdating) {
            isUpdating = false
        }
    }

    private suspend fun fetchTokenAndWrapErrors(): GetTokenResponseData {
        return suspendCoroutine { continuation ->
            launch {
                var resp: GetTokenResponseData
                try {
                    resp = fetchToken()
                    synchronized(tokenLock) {
                        token = resp.identificateur
                    }
                } catch (e: VolleyError) {
                    val msg: String = when (e.networkResponse?.statusCode ?: -1) {
                        400 -> appCtx.getString(R.string.error_message_bad_request)
                        403 -> appCtx.getString(R.string.error_message_forbidden)
                        500 -> appCtx.getString(R.string.error_message_server)
                        else -> appCtx.getString(R.string.error_message_unknown) + ": $e"
                    }
                    resp = GetTokenResponseData(0, msg)
                } catch (e: Exception) {
                    val msg = appCtx.getString(R.string.error_message_network)
                    resp = GetTokenResponseData(0, "$msg: ${e.localizedMessage}]")
                }
                continuation.resume(resp)
            }
        }
    }

    /*
     * We can't put this function in RestRequestService because it would create circular dependencies.
     */
    private suspend fun fetchToken(): GetTokenResponseData {
        val manager = appCtx.getSystemService(Context.WIFI_SERVICE) as WifiManager?
        if (manager != null && manager.isWifiEnabled) {
            val info = manager.connectionInfo
            val ip = NetUtils.translateIP(info.ipAddress)
            val mac = NetUtils.getMacAddress(appCtx.getString(R.string.interface_name_wifi))
            return suspendCoroutine { continuation ->
                val request = RESTRequest(
                        Request.Method.GET,
                        httpClient.getBaseURL() + RESOURCE_URI,
                        Gson().toJson(GetTokenRequestData(
                                ip,
                                mac,
                                preferences.getString(PREFERENCE_KEY_USERNAME, "")!!
                        )),
                        GetTokenResponseData::class.java,
                        mutableMapOf(HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()), // Headers
                        Response.Listener { respData: ResponseData<GetTokenResponseData> -> // Success listener
                            continuation.resume(respData.value) // Equivalent to Promise.resolve(value)
                        },
                        Response.ErrorListener { error -> // Error Listener
                            error.printStackTrace()
                            continuation.resumeWithException(error) // Equivalent to Promise.reject(error)
                        }
                )
                // Set the request timeout
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
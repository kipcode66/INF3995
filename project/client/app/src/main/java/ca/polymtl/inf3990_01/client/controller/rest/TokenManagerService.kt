package ca.polymtl.inf3990_01.client.controller.rest

import android.content.SharedPreferences
import android.util.MalformedJsonException
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import com.android.volley.Request
import com.android.volley.Response
import com.google.gson.Gson
import kotlinx.coroutines.experimental.launch
import kotlin.coroutines.experimental.suspendCoroutine

class TokenManagerService(private val httpClient: HTTPRestClient, private val preferences: SharedPreferences) {
    private class GetTokenRequestData(val ip: String, val MAC: String, val nom: String)
    private class GetTokenResponseData(val identificateur: Int, val message: String)

    private var token: Int = 0
    suspend fun getToken(): Int {
        // TODO Ensure that the token is still valid.
        val resp = fetchToken()
        token = resp.identificateur
        return token
    }

    private suspend fun fetchToken(): GetTokenResponseData {
        return suspendCoroutine { continuation ->
            httpClient.addToRequestQueue(RESTRequest(
                    Request.Method.GET,
                    httpClient.getBaseURL() + "/usager/identification",
                    Gson().toJson(GetTokenRequestData("0.0.0.0", "00:00:00:00:00:00", preferences.getString("client_name", "") ?: "")),
                    GetTokenResponseData::class.java,
                    mutableMapOf("Auth-Token" to token.toString()),
                    Response.Listener{ respData: ResponseData<GetTokenResponseData> ->
                        continuation.resume(respData.value)
                    },
                    Response.ErrorListener { error ->
                        error.printStackTrace()
                        continuation.resumeWithException(error)
                    }
            ))
        }
    }
}
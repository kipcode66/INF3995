package ca.polymtl.inf3990_01.client.controller.rest

import android.content.Context
import android.os.Handler
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.InitializationManager
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import ca.polymtl.inf3990_01.client.model.*
import com.android.volley.DefaultRetryPolicy
import com.android.volley.Request
import com.android.volley.Response
import kotlin.coroutines.experimental.suspendCoroutine

class SecureRestRequestService(
    private val appCtx: Context,
    private val tokenMgr: TokenManagerService,
    private val httpsClient: HTTPSRestClient,
    private val initMgr: InitializationManager,
    private val dataProvider: DataProvider
    ) {
        companion object {
            private class userResponseData(
                   val mac: String, val ip: String, val name: String)
            private class UserListResponseData(val users: List<userResponseData>)

        }

    suspend fun getSongList(): List<Song> {
        TODO("Not Implemented")
    }



    suspend fun swapSongs(pair: Pair<Song, Song>) {
        TODO("Not Implemented")
    }

    suspend fun getVolume(): SoundVolume {
        TODO("Not Implemented")
    }

    suspend fun getStatistics(): Statistics {
        TODO("Not Implemented")
    }


    suspend fun blockUser(user: User) {
        TODO("Not Implemented")
    }

    suspend fun unblockUser(user: User) {
        TODO("Not Implemented")
    }

    suspend fun deleteSong(song: Song) {
        TODO("Not Implemented")
    }

    suspend fun getBlackList(): List<User> {
        val list: MutableList<User> = mutableListOf()
        val token = tokenMgr.getToken()
        val resp: ResponseData<UserListResponseData> = suspendCoroutine { continuation ->
            var canDisplayMessage = initMgr.isInitialized
            val request = RESTRequest<UserListResponseData>(
                    Request.Method.GET,
                    httpsClient.getBaseURL() + RestRequestService.RESOURCE_URI + token,
                    "",
                    UserListResponseData::class.java,
                    mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                    Response.Listener { resp ->
                        continuation.resume(resp)
                    },
                    Response.ErrorListener { error ->
                        val msg = when (error.networkResponse?.statusCode ?: 0) {
                            401 -> appCtx.getString(R.string.error_message_unknow_user)
                            else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage}"
                        }
                        if (canDisplayMessage) {
                            Handler(appCtx.mainLooper).post {
                                Toast.makeText(appCtx, msg, Toast.LENGTH_LONG).show()
                            }
                        }
                        val resp = ResponseData(error.networkResponse?.statusCode ?: 0, UserListResponseData(listOf()), error.networkResponse)
                        continuation.resume(resp)
                    }
            )
            request.setRetryPolicy(DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 2, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT))
            httpsClient.addToRequestQueue(request)
        }
        for (user in resp.value.users) {
            list.add(User(user.mac, user.ip,user.name))
        }
        return list
    }
}
package ca.polymtl.inf3990_01.client.controller.rest

import android.content.Context
import android.os.Handler
import android.util.Log
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.InitializationManager
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import com.android.volley.DefaultRetryPolicy
import com.android.volley.Request
import com.android.volley.Response
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.model.SoundVolume
import ca.polymtl.inf3990_01.client.model.Statistics
import ca.polymtl.inf3990_01.client.model.User
import com.google.gson.Gson
import java.math.BigInteger
import kotlin.coroutines.experimental.suspendCoroutine

class SecureRestRequestService(
    private val appCtx: Context,
    private val httpsClient: HTTPSRestClient,
    private val tokenService: TokenManagerService,
    private val initMgr: InitializationManager,
    private val appStateService: AppStateService
    ) {
        companion object {
            private class userResponseData(
                   val mac: String, val ip: String, val name: String)
            private class UserListResponseData(val users: List<userResponseData>)
            data class LoginRequestData(val usager: String, val mot_de_passe: String)
            private class SongResponseData(
                    val titre: String, val artiste: String, val duree: String,
                    val ip: String, val mac: String, val id: BigInteger,
                    val proposeePar: String?, val proprietaire: Boolean, val no: Int)
            private class SongListResponseData(val chansons: List<SongResponseData>)

            const val RESOURCE_URI = "/superviseur/file"

        }
    private var lastMessageSongList: String? = null


    suspend fun getSongList(): List<Song> {
        val list: MutableList<Song> = mutableListOf()
        val token = tokenService.getToken()
        val resp: ResponseData<SongListResponseData> = suspendCoroutine { continuation ->
            var canDisplayMessage = initMgr.isInitialized
            val request = RESTRequest<SongListResponseData>(
                Request.Method.GET,
                httpsClient.getBaseURL() + RESOURCE_URI,
                "",
                SongListResponseData::class.java,
                mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                Response.Listener { resp ->
                    continuation.resume(resp)
                },
                Response.ErrorListener { error ->
                    val msg = when (error.networkResponse?.statusCode ?: 0) {
                        401 -> appCtx.getString(R.string.error_message_unauthenticated)
                        500 -> appCtx.getString(R.string.error_message_server)
                        else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage}"
                    }
                    // TODO Send a signal to the Presenter to show popup with the response message.
                    // Temporarly, opening a Toast (a little message at the bottom of the screen)
                    if (canDisplayMessage && lastMessageSongList != msg) {
                        lastMessageSongList = msg
                        Handler(appCtx.mainLooper).post {
                            Toast.makeText(appCtx, msg, Toast.LENGTH_LONG).show()
                        }
                    }
                    val resp = ResponseData(error.networkResponse?.statusCode ?: 0, SongListResponseData(listOf()), error.networkResponse)
                    continuation.resume(resp)
                }
            )
            request.setRetryPolicy(DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 2, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT))
            httpsClient.addToRequestQueue(request)
        }
        for (chanson in resp.value.chansons) {
            val duration = chanson.duree.split(":").map(String::toInt).reduce { acc, i -> acc * 60 + i } * 1000
            list.add(Song(chanson.titre, chanson.artiste, duration, chanson.no, if (chanson.proprietaire) null else (chanson.proposeePar ?: ""), chanson.ip, chanson.mac, chanson.id))
        }
        return list
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
        val token = tokenService.getToken()
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
                        val resp = ResponseData(error.networkResponse?.statusCode
                                ?: 0, UserListResponseData(listOf()), error.networkResponse)
                        continuation.resume(resp)
                    }
            )
            request.setRetryPolicy(DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 2, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT))
            httpsClient.addToRequestQueue(request)
        }
        for (user in resp.value.users) {
            list.add(User(user.mac, user.ip, user.name))
        }
        return list
    }

    suspend fun login(username: String, password: String) {
        Log.d("SecureRestRequest.login", "[login=$username;password=$password]")
        val resp: Boolean = suspendCoroutine { continuation ->
            val canDisplayMessage = initMgr.isInitialized
            val token = tokenService.getToken()
            val request = RESTRequest(
                Request.Method.POST,
                httpsClient.getBaseURL() + "/superviseur/login",
                Gson().toJson(LoginRequestData(username, password)),
                String::class.java,
                mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                Response.Listener {
                    continuation.resume(true)
                },
                Response.ErrorListener { error ->
                    val msg = when (error.networkResponse?.statusCode) {
                        400 -> appCtx.getString(R.string.error_message_bad_request)
                        403 -> appCtx.getString(R.string.error_message_forbidden)
                        else -> {
                            error.printStackTrace()
                            appCtx.getString(R.string.error_message_unknown) + "; ${error.message}"
                        }
                    }
                    if (canDisplayMessage) {
                        Handler(appCtx.mainLooper).post {
                            Toast.makeText(appCtx, msg, Toast.LENGTH_LONG).show()
                        }
                    }
                    continuation.resume(false)
                }
            )
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        if (resp) {
            appStateService.setState(AppStateService.State.Admin)
        }
    }

    suspend fun logout() {
        val resp: Boolean = suspendCoroutine { continuation ->
            val canDisplayMessage = initMgr.isInitialized
            val token = tokenService.getToken()
            val request = RESTRequest(
                Request.Method.POST,
                httpsClient.getBaseURL() + "/superviseur/logout",
                "",
                String::class.java,
                mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                Response.Listener {
                    continuation.resume(true)
                },
                Response.ErrorListener { error ->
                    val msg = when (error.networkResponse?.statusCode) {
                        401 -> appCtx.getString(R.string.error_message_unauthenticated)
                        else -> {
                            error.printStackTrace()
                            appCtx.getString(R.string.error_message_unknown) + "; ${error.message}"
                        }
                    }
                    if (canDisplayMessage) {
                        Handler(appCtx.mainLooper).post {
                            Toast.makeText(appCtx, msg, Toast.LENGTH_LONG).show()
                        }
                    }
                    continuation.resume(false)
                }
            )
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 1, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        appStateService.setState(AppStateService.State.User)
    }

    suspend fun changePassword(newPassword: String) {
        TODO("Not Implemented")
    }
}
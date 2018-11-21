package ca.polymtl.inf3990_01.client.controller.rest

import android.content.Context
import android.os.Handler
import android.util.Log
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.InitializationManager
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.RequestQueueReloadEvent
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.Song
import ca.polymtl.inf3990_01.client.model.SoundVolume
import ca.polymtl.inf3990_01.client.model.Statistics
import ca.polymtl.inf3990_01.client.model.User
import com.android.volley.DefaultRetryPolicy
import com.android.volley.Request
import com.android.volley.Response
import com.google.gson.Gson
import java.math.BigInteger
import kotlin.coroutines.experimental.suspendCoroutine

class SecureRestRequestService(
    private val appCtx: Context,
    private val httpsClient: HTTPSRestClient,
    private val tokenService: TokenManagerService,
    private val initMgr: InitializationManager,
    private val eventMgr: EventManager,
    private val appStateService: AppStateService
    ) {
    companion object {
        private data class UserResponseData(
            val mac: String, val ip: String, val nom: String)
        private data class UserListResponseData(val bloques: List<UserResponseData>)

        private data class LoginRequestData(val usager: String, val mot_de_passe: String)

        private data class SongResponseData(
            val titre: String, val artiste: String, val duree: String,
            val ip: String, val mac: String, val id: BigInteger,
            val proposeePar: String?, val proprietaire: Boolean, val no: Int)
        private data class SongListResponseData(val chansons: List<SongResponseData>)

        private data class SwapSongRequestData(val une: Int, val autre: Int)

        private const val RESOURCE_URI = "/superviseur"
    }
    private var lastMessageSongList: String? = null

    suspend fun getSongList(): List<Song> {
        val list: MutableList<Song> = mutableListOf()
        val token = tokenService.getToken()
        val resp: ResponseData<SongListResponseData> = suspendCoroutine { continuation ->
            val canDisplayMessage = initMgr.isInitialized
            val request = RESTRequest(
                Request.Method.GET,
                httpsClient.getBaseURL() + "$RESOURCE_URI/file/",
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
                    // lastMessageSongList is used to prevent having the same message spamming the user.
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
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        for (chanson in resp.value.chansons) {
            val duration = chanson.duree.split(":").map(String::toInt).reduce { acc, i -> acc * 60 + i } * 1000
            list.add(Song(chanson.titre, chanson.artiste, duration, chanson.no, if (chanson.proprietaire) null else (chanson.proposeePar ?: ""), chanson.ip, chanson.mac, chanson.id))
        }
        return list
    }

    suspend fun deleteSong(song: Song) {
        val token = tokenService.getToken()
        val songToDelete = song.id.toString()
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = RESTRequest(
                    Request.Method.DELETE,
                    httpsClient.getBaseURL() + "$RESOURCE_URI/chanson/$songToDelete",
                    "",
                    String::class.java,
                    mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                    Response.Listener { resp ->
                        continuation.resume(resp)
                    },
                    Response.ErrorListener { error ->
                        val msg = when (error.networkResponse?.statusCode ?: 0) {
                            401 -> appCtx.getString(R.string.error_message_unknown_user)
                            405 -> appCtx.getString(R.string.error_message_deletion_refused)
                            else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage?:error.message}"
                        }
                        continuation.resume(ResponseData(error.networkResponse?.statusCode ?: 0, msg, error.networkResponse))
                    }
            )
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
    }

    suspend fun swapSongs(pair: Pair<Song, Song>) {
        val token = tokenService.getToken()
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = RESTRequest(
                Request.Method.POST,
                httpsClient.getBaseURL() + "$RESOURCE_URI/inversion",
                Gson().toJson(SwapSongRequestData(pair.first.id, pair.second.id)),
                String::class.java,
                mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                Response.Listener { resp ->
                    continuation.resume(resp)
                },
                Response.ErrorListener { error ->
                    val msg = when (error.networkResponse?.statusCode ?: 0) {
                        400 -> appCtx.getString(R.string.error_message_bad_request)
                        401 -> appCtx.getString(R.string.error_message_unknown_user)
                        405 -> appCtx.getString(R.string.error_message_unauthenticated)
                        409 -> appCtx.getString(R.string.error_message_song_not_in_queue)
                        else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage?:error.message}"
                    }
                    continuation.resume(ResponseData(error.networkResponse?.statusCode ?: 0, msg, error.networkResponse))
                }
            )
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
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

    suspend fun getBlackList(): List<User> {
        val list: MutableList<User> = mutableListOf()
        val token = tokenService.getToken()
        val resp: ResponseData<UserListResponseData> = suspendCoroutine { continuation ->
            val canDisplayMessage = initMgr.isInitialized
            val request = RESTRequest(
                    Request.Method.GET,
                    httpsClient.getBaseURL() + "$RESOURCE_URI/listenoire" ,
                    "",
                    UserListResponseData::class.java,
                    mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                    Response.Listener { resp ->
                        continuation.resume(resp)
                    },
                    Response.ErrorListener { error ->
                        val msg = when (error.networkResponse?.statusCode ?: 0) {
                            401 -> appCtx.getString(R.string.error_message_unknown_user)
                            else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage?:error.message}"
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
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        for (user in resp.value.bloques) {
            list.add(User(user.mac, user.ip, user.nom))
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
                httpsClient.getBaseURL() + "$RESOURCE_URI/login",
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
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        if (resp) {
            appStateService.setState(AppStateService.State.Admin)
        }
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
    }

    suspend fun logout() {
        suspendCoroutine<Boolean> { continuation ->
            val canDisplayMessage = initMgr.isInitialized
            val token = tokenService.getToken()
            val request = RESTRequest(
                Request.Method.POST,
                httpsClient.getBaseURL() + "$RESOURCE_URI/logout",
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
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        appStateService.setState(AppStateService.State.User)
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
    }

    suspend fun changePassword(newPassword: String) {
        TODO("Not Implemented")
    }
}
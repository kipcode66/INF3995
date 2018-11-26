package ca.polymtl.inf3990_01.client.controller.rest

import android.content.Context
import android.os.Handler
import android.util.Log
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.InitializationManager
import ca.polymtl.inf3990_01.client.controller.VolumeController
import ca.polymtl.inf3990_01.client.controller.event.EventManager
import ca.polymtl.inf3990_01.client.controller.event.RequestBlackListReloadEvent
import ca.polymtl.inf3990_01.client.controller.event.RequestQueueReloadEvent
import ca.polymtl.inf3990_01.client.controller.event.VolumeRequestEvent
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import ca.polymtl.inf3990_01.client.controller.state.AppStateService
import ca.polymtl.inf3990_01.client.model.*
import com.android.volley.DefaultRetryPolicy
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.HttpHeaderParser
import com.google.gson.Gson
import java.math.BigInteger
import java.nio.charset.Charset
import java.util.concurrent.TimeUnit
import kotlin.coroutines.experimental.Continuation
import kotlin.coroutines.experimental.suspendCoroutine

class SecureRestRequestService(
    private val appCtx: Context,
    private val httpsClient: HTTPSRestClient,
    private val tokenService: TokenManagerService,
    private val initMgr: InitializationManager,
    private val eventMgr: EventManager,
    private val dataProvider: DataProvider,
    private val appStateService: AppStateService,
    private val volumeController: VolumeController
    ) {
    companion object {
        private data class UserData(
            val mac: String, val ip: String, val nom: String)
        private data class UserListResponseData(val bloques: List<UserData>)

        private data class LoginRequestData(val usager: String, val mot_de_passe: String)

        private data class SongResponseData(
            val titre: String, val artiste: String, val duree: String,
            val ip: String, val mac: String, val id: BigInteger,
            val proposeePar: String?, val proprietaire: Boolean, val no: Int)
        private data class SongListResponseData(val chansons: List<SongResponseData>)

        private data class StatisticsResponseData(
            val chansons: BigInteger, val utilisateurs: BigInteger,
            val elimines: BigInteger, val temps: String)

        private data class SwapSongRequestData(val une: Int, val autre: Int)

        private data class VolumeResponseData(val volume: BigInteger, val sourdine: Boolean)

        private data class ChangePasswordRequestData(val ancien: String, val nouveau: String)

        private const val RESOURCE_URI = "/superviseur"
    }
    private var lastMessageGeneric: String? = null

    private inline fun <reified T> generateRequest(method: Int, res: String, body: Any, continuation: Continuation<ResponseData<T>>, messages: MutableMap<Int, String>, defaultResponse: T, displayToast: Boolean = false): RESTRequest<T> {
        val canDisplayMessage = initMgr.isInitialized
        return RESTRequest(method, httpsClient.getBaseURL() + "$RESOURCE_URI/$res", Gson().toJson(body), T::class.java,
            mutableMapOf(
                TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to tokenService.getToken().toString()
            ),
            Response.Listener { resp ->
                continuation.resume(resp)
            },
            Response.ErrorListener { error ->
                val code: Int = error.networkResponse?.statusCode ?: 0
                val msg = when {
                    messages.keys.indexOf(code) > 0 -> messages[code] + " [" + String(
                        error.networkResponse?.data ?: ByteArray(0),
                        Charset.forName(HttpHeaderParser.parseCharset(error.networkResponse?.headers))) + "]"
                    else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage}"
                }
                // lastMessageGeneric is used to prevent having the same message spamming the user.
                if (displayToast && canDisplayMessage && lastMessageGeneric != msg) {
                    lastMessageGeneric = msg
                    Handler(appCtx.mainLooper).post {
                        Toast.makeText(appCtx, msg, Toast.LENGTH_LONG).show()
                    }
                }
                val resp = ResponseData(error.networkResponse?.statusCode ?: 0, defaultResponse, error.networkResponse)
                continuation.resume(resp)
            })
    }

    suspend fun getSongList(): List<Song> {
        val list: MutableList<Song> = mutableListOf()
        val resp: ResponseData<SongListResponseData> = suspendCoroutine { continuation ->
            val request = generateRequest(Request.Method.GET, "file/", "", continuation, mutableMapOf(
                400 to appCtx.getString(R.string.error_message_bad_request),
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), SongListResponseData(listOf()), true)
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
        val songToDelete = song.id.toString()
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(
                Request.Method.DELETE, "chanson/$songToDelete",
                "", continuation, mutableMapOf(
                    400 to appCtx.getString(R.string.error_message_bad_request),
                    401 to appCtx.getString(R.string.error_message_unauthenticated),
                    500 to appCtx.getString(R.string.error_message_server)
                ), "")
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
    }

    suspend fun swapSongs(pair: Pair<Song, Song>) {
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(
                Request.Method.POST, "inversion", SwapSongRequestData(pair.first.id, pair.second.id), continuation, mutableMapOf(
                    400 to appCtx.getString(R.string.error_message_bad_request),
                    401 to appCtx.getString(R.string.error_message_unknown_user),
                    405 to appCtx.getString(R.string.error_message_unauthenticated),
                    409 to appCtx.getString(R.string.error_message_song_not_in_queue),
                    500 to appCtx.getString(R.string.error_message_server)
            ), "")
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
    }

    suspend fun getVolume(): Volume {
        var volume = volumeController.getVolume()
        val resp: ResponseData<VolumeResponseData> = suspendCoroutine { continuation ->
            val request = generateRequest(Request.Method.GET, "volume", "", continuation, mutableMapOf(
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), VolumeResponseData(BigInteger.ZERO, false))
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        if (resp.code == 200) {
            volume = Volume(resp.value.volume.toInt(), resp.value.sourdine)
        }
        return volume
    }

    suspend fun setVolume(pc: Int) {
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(Request.Method.POST, "volume/assigner/$pc", "", continuation, mutableMapOf(
                400 to appCtx.getString(R.string.error_message_bad_request),
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), "")
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(VolumeRequestEvent())
    }

    suspend fun muteVolume() {
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(Request.Method.POST, "volume/sourdine/activer", "", continuation, mutableMapOf(
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), "")
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(VolumeRequestEvent())
    }

    suspend fun unmuteVolume() {
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(Request.Method.POST, "volume/sourdine/desactiver", "", continuation, mutableMapOf(
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), "")
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(VolumeRequestEvent())
    }

    suspend fun getStatistics(): Statistics {
        var stat = dataProvider.getStatistics()
        val resp: ResponseData<StatisticsResponseData> = suspendCoroutine { continuation ->
            val request : RESTRequest<StatisticsResponseData> =  generateRequest(Request.Method.GET, "statistiques/", "", continuation, mutableMapOf(
                    401 to appCtx.getString(R.string.error_message_unauthenticated)
            ), StatisticsResponseData(BigInteger.ZERO, BigInteger.ZERO, BigInteger.ZERO, "00:00"),true)
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        if (resp.code == 200) {
            val meanTime = resp.value.temps.split(":").map(String::toInt).reduce { acc, i -> acc * 60 + i } * 1000
            stat = Statistics(resp.value.chansons, resp.value.utilisateurs, resp.value.elimines, BigInteger.valueOf(meanTime.toLong()))
        }
        return stat
    }

    suspend fun blockUser(user: User) {
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(Request.Method.POST, "bloquer", UserData(user.mac, user.ip, user.name), continuation, mutableMapOf(
                400 to appCtx.getString(R.string.error_message_bad_request),
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), "")
            request.retryPolicy = DefaultRetryPolicy(TimeUnit.SECONDS.toMillis(10L).toInt(), 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(RequestBlackListReloadEvent())
    }

    suspend fun unblockUser(user: User) {
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(Request.Method.POST, "debloquer", UserData(user.mac, user.ip, user.name), continuation, mutableMapOf(
                400 to appCtx.getString(R.string.error_message_bad_request),
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), "")
            request.retryPolicy = DefaultRetryPolicy(TimeUnit.SECONDS.toMillis(10L).toInt(), 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(RequestBlackListReloadEvent())
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

    suspend fun changePassword(oldPassword: String, newPassword: String) {
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = generateRequest(Request.Method.POST, "changement_mdp", ChangePasswordRequestData(oldPassword, newPassword), continuation, mutableMapOf(
                400 to appCtx.getString(R.string.error_message_bad_request),
                401 to appCtx.getString(R.string.error_message_unauthenticated),
                500 to appCtx.getString(R.string.error_message_server)
            ), "", true)
            request.retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpsClient.addToRequestQueue(request)
        }
    }
}
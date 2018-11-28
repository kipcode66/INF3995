package ca.polymtl.inf3995.client.controller.rest

import android.content.Context
import android.os.Handler
import android.widget.Toast
import ca.polymtl.inf3995.client.R
import ca.polymtl.inf3995.client.controller.InitializationManager
import ca.polymtl.inf3995.client.controller.event.EventManager
import ca.polymtl.inf3995.client.controller.event.RequestQueueReloadEvent
import ca.polymtl.inf3995.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3995.client.controller.rest.requests.ResponseData
import ca.polymtl.inf3995.client.model.DataProvider
import ca.polymtl.inf3995.client.model.LocalSong
import ca.polymtl.inf3995.client.model.Song
import ca.polymtl.inf3995.client.utils.NetUtils
import com.android.volley.DefaultRetryPolicy
import com.android.volley.Request
import com.android.volley.Response
import java.io.InputStreamReader
import java.math.BigInteger
import kotlin.coroutines.experimental.suspendCoroutine


class RestRequestService(
    private val appCtx: Context,
    private val tokenMgr: TokenManagerService,
    private val httpClient: HTTPRestClient,
    private val initMgr: InitializationManager,
    private val eventMgr: EventManager,
    private val dataProvider: DataProvider
) {
    companion object {
        private class SongResponseData(
            val titre: String, val artiste: String, val duree: String,
            val proposeePar: String?, val proprietaire: Boolean, val no: Int
        )

        private class SongListResponseData(val chansons: List<SongResponseData>)

        const val RESOURCE_URI = "/usager"
        private const val DEFAULT_SEND_TIMEOUT_MS = 60 * 1000
    }

    private var lastMessageSongList: String? = null
    private var lastMessageSendSong: String? = null

    suspend fun getSongList(): List<Song> {
        val list: MutableList<Song> = mutableListOf()
        val token = tokenMgr.getToken()
        val resp: ResponseData<SongListResponseData> = suspendCoroutine { continuation ->
            val canDisplayMessage = initMgr.isInitialized && token != BigInteger.ZERO
            val request = RESTRequest(
                Request.Method.GET,
                httpClient.getBaseURL() + "$RESOURCE_URI/file/" + token,
                "",
                SongListResponseData::class.java,
                mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                Response.Listener { resp ->
                    continuation.resume(resp)
                },
                Response.ErrorListener { error ->
                    val msg = when (error.networkResponse?.statusCode ?: 0) {
                        403  -> appCtx.getString(R.string.error_message_forbidden)
                        500  -> appCtx.getString(R.string.error_message_server)
                        else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage}"
                    }
                    // Temporarily, opening a Toast (a little message at the bottom of the screen)
                    if (canDisplayMessage && lastMessageSongList != msg) {
                        lastMessageSongList = msg
                        Handler(appCtx.mainLooper).post {
                            Toast.makeText(appCtx, msg, Toast.LENGTH_LONG).show()
                        }
                    }
                    val resp = ResponseData(error.networkResponse?.statusCode
                                            ?: 0, SongListResponseData(listOf()), error.networkResponse)
                    continuation.resume(resp)
                }
            )
            request
                .retryPolicy = DefaultRetryPolicy(DefaultRetryPolicy.DEFAULT_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpClient.addToRequestQueue(request)
        }
        for (chanson in resp.value.chansons) {
            val duration = chanson.duree.split(":").map(String::toInt).reduce { acc, i -> acc * 60 + i } * 1000
            list
                .add(Song(chanson.titre, chanson.artiste, duration, chanson.no, if (chanson.proprietaire) null else (chanson.proposeePar
                                                                                                                     ?: "")))
        }
        return list
    }

    @Synchronized
    suspend fun sendSong(song: LocalSong) {
        val songToSend = NetUtils.encoder(song)
        val token = tokenMgr.getToken()
        val canDisplayMessage = initMgr.isInitialized
        val resp: ResponseData<String> = suspendCoroutine { continuation ->
            val request = RESTRequest(
                Request.Method.POST,
                httpClient.getBaseURL() + "$RESOURCE_URI/chanson/$token",
                songToSend.reader().use(InputStreamReader::readText),
                String::class.java,
                mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                Response.Listener { resp ->
                    continuation.resume(resp)
                },
                Response.ErrorListener { error ->
                    val msg = when (error.networkResponse?.statusCode ?: 0) {
                        403  -> appCtx.getString(R.string.error_message_forbidden)
                        413  -> appCtx.getString(R.string.error_message_server)
                        415  -> appCtx.getString(R.string.error_message_server)
                        500  -> appCtx.getString(R.string.error_message_server)
                        else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage}"
                    }
                    continuation.resume(ResponseData(error.networkResponse?.statusCode
                                                     ?: 0, msg, error.networkResponse))
                }
            )
            request
                .retryPolicy = DefaultRetryPolicy(DEFAULT_SEND_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpClient.addToRequestQueue(request)
        }
        if (resp.code != 200) {
            synchronized(dataProvider) {
                if (dataProvider[song] <= DataProvider.LocalSongSendState.SENDING) {
                    dataProvider[song] = DataProvider.LocalSongSendState.NOT_SENT
                }
            }
            if (canDisplayMessage && lastMessageSendSong != resp.value) {
                lastMessageSendSong = resp.value
                Handler(appCtx.mainLooper).post {
                    Toast.makeText(appCtx, "Post song error: " + resp.value, Toast.LENGTH_LONG)
                        .show()
                }
            }
        } else {
            synchronized(dataProvider) {
                if (dataProvider[song] <= DataProvider.LocalSongSendState.SENDING) {
                    dataProvider[song] = DataProvider.LocalSongSendState.WAITING_FOR_SERVER_QUEUE
                }
            }
        }
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
    }

    suspend fun deleteSong(song: Song) {
        val token = tokenMgr.getToken()
        val songToDelete = song.id.toString()
        suspendCoroutine<ResponseData<String>> { continuation ->
            val request = RESTRequest(
                Request.Method.DELETE,
                httpClient.getBaseURL() + "$RESOURCE_URI/chanson/$token/$songToDelete",
                "",
                String::class.java,
                mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                Response.Listener { resp ->
                    continuation.resume(resp)
                },
                Response.ErrorListener { error ->
                    val msg = when (error.networkResponse?.statusCode ?: 0) {
                        403  -> appCtx.getString(R.string.error_message_server)
                        405  -> appCtx.getString(R.string.error_message_deletion_refused)
                        else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage}"
                    }
                    continuation.resume(ResponseData(error.networkResponse?.statusCode
                                                     ?: 0, msg, error.networkResponse))
                }
            )
            request
                .retryPolicy = DefaultRetryPolicy(DEFAULT_SEND_TIMEOUT_MS, 0, DefaultRetryPolicy.DEFAULT_BACKOFF_MULT)
            httpClient.addToRequestQueue(request)
        }
        eventMgr.dispatchEvent(RequestQueueReloadEvent())
    }
}

package ca.polymtl.inf3990_01.client.controller.rest

import android.content.Context
import android.os.Handler
import android.widget.Toast
import ca.polymtl.inf3990_01.client.R
import ca.polymtl.inf3990_01.client.controller.rest.requests.RESTRequest
import ca.polymtl.inf3990_01.client.controller.rest.requests.ResponseData
import ca.polymtl.inf3990_01.client.model.LocalSong
import ca.polymtl.inf3990_01.client.model.Song
import com.android.volley.Request
import com.android.volley.Response
import kotlin.coroutines.experimental.suspendCoroutine

class RestRequestService(private val appCtx: Context, private val tokenMgr: TokenManagerService, private val httpClient: HTTPRestClient) {
    companion object {
        private class SongResponseData(
                val titre: String, val artiste: String, val duree: String,
                val proposeePar: String?, val proprietaire: Boolean, val no: Int)
        private class SongListResponseData(val chansons: List<SongResponseData>)

        const val RESOURCE_URI = "/usager/file/"
    }

    suspend fun getSongList(): List<Song> {
        val list: MutableList<Song> = mutableListOf()
        val token = tokenMgr.getToken()
        val resp: ResponseData<SongListResponseData> = suspendCoroutine { continuation ->
            val request = RESTRequest<SongListResponseData>(
                    Request.Method.GET,
                    httpClient.getBaseURL() + RESOURCE_URI + token,
                    "",
                    SongListResponseData::class.java,
                    mutableMapOf(TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN to token.toString()),
                    Response.Listener { resp ->
                        continuation.resume(resp)
                    },
                    Response.ErrorListener { error ->
                        val msg = when (error.networkResponse?.statusCode ?: 0) {
                            403 -> appCtx.getString(R.string.error_message_forbidden)
                            500 -> appCtx.getString(R.string.error_message_server)
                            else -> appCtx.getString(R.string.error_message_unknown) + "; ${error.localizedMessage}"
                        }
                        // TODO Send a signal to the Presenter to show popup with the response message.
                        // Temporarly, opening a Toast (a little message at the bottom of the screen)
                        Handler(appCtx.mainLooper).post {
                            Toast.makeText(appCtx, msg, Toast.LENGTH_LONG).show()
                        }
                        val resp = ResponseData(error.networkResponse?.statusCode ?: 0, SongListResponseData(listOf()))
                        continuation.resume(resp)
                    }
            )
            httpClient.addToRequestQueue(request)
        }
        for (chanson in resp.value.chansons) {
            val duration = chanson.duree.split(":").map(String::toInt).reduce { acc, i -> acc * 60 + i }
            list.add(Song(chanson.titre, chanson.artiste, duration, chanson.no, if (chanson.proprietaire) null else (chanson.proposeePar ?: "")))
        }
        return list
    }

    suspend fun sendSong(song: LocalSong) {
        TODO("Not Implemented")
    }

    suspend fun deleteSong(song: Song) {
        TODO("Not Implemented")
    }
}

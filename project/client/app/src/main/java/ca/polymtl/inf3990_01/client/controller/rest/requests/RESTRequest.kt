package ca.polymtl.inf3990_01.client.controller.rest.requests

import android.support.annotation.CallSuper
import ca.polymtl.inf3990_01.client.controller.InitializationManager
import ca.polymtl.inf3990_01.client.controller.rest.TokenManagerService
import com.android.volley.NetworkResponse
import com.android.volley.ParseError
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.HttpHeaderParser
import com.google.gson.Gson
import com.google.gson.JsonSyntaxException
import kotlinx.coroutines.experimental.launch
import java.io.UnsupportedEncodingException
import java.nio.charset.Charset

class RESTRequest<T>(
        method: Int,
        url: String,
        private val body: String = "",
        private val responseClass: Class<T>?,
        private val headers: MutableMap<String, String>?,
        private var listener: Response.Listener<ResponseData<T?>>,
        errorListener: Response.ErrorListener?
) : Request<ResponseData<T?>>(method, url, Response.ErrorListener {error ->
    if (error.networkResponse?.statusCode == CODE_FORBIDDEN && TokenManagerService.hasInstance()) {
        val tokenService = TokenManagerService.getInstanceOrThrow()
        launch {
            tokenService.updateToken()
            errorListener?.onErrorResponse(error)
        }
    }
    else {
        errorListener?.onErrorResponse(error)
    }
}) {
    companion object { const val CODE_FORBIDDEN = 403 }
    private val DUMMY_LISTENER = Response.Listener<ResponseData<T?>>{ _ -> }
    private val gson = Gson()

    override fun getHeaders(): MutableMap<String, String> {
        val h = headers ?: super.getHeaders()
        var token = 0
        if (TokenManagerService.hasInstance()) {
            token = TokenManagerService.getInstanceOrThrow().getToken()
        }
        h[TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN] = h[TokenManagerService.HTTP_HEADER_NAME_X_AUTH_TOKEN] ?: token.toString()
        return h
    }

    override fun getBody(): ByteArray = body.toByteArray()

    @CallSuper
    override fun cancel() {
        super.cancel()
        listener = DUMMY_LISTENER
    }

    override fun deliverResponse(response: ResponseData<T?>) = listener.onResponse(response)

    override fun parseNetworkResponse(response: NetworkResponse?): Response<ResponseData<T?>> {
        return try {
            val json = String(
                    response?.data ?: ByteArray(0),
                    Charset.forName(HttpHeaderParser.parseCharset(response?.headers)))
            Response.success(
                    ResponseData(response?.statusCode ?: -1, if (responseClass!=null) gson.fromJson(json, responseClass) else null,
                        response),
                    HttpHeaderParser.parseCacheHeaders(response))
        } catch (e: UnsupportedEncodingException) {
            Response.error(ParseError(e))
        } catch (e: JsonSyntaxException) {
            Response.error(ParseError(e))
        }
    }
}
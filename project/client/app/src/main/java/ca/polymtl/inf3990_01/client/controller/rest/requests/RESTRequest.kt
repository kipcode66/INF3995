package ca.polymtl.inf3990_01.client.controller.rest.requests

import com.android.volley.NetworkResponse
import com.android.volley.ParseError
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.HttpHeaderParser
import com.google.gson.Gson
import com.google.gson.JsonSyntaxException
import java.io.UnsupportedEncodingException
import java.nio.charset.Charset

class RESTRequest<T>(
        method: Int,
        url: String,
        private val body: String = "",
        private val clazz: Class<T>,
        private val headers: MutableMap<String, String>?,
        private val listener: Response.Listener<ResponseData<T>>,
        errorListener: Response.ErrorListener?
) : Request<ResponseData<T>>(method, url, errorListener) {
    private val gson = Gson()

    override fun getHeaders(): MutableMap<String, String> = headers ?: super.getHeaders()

    override fun getBody(): ByteArray = body.toByteArray()

    override fun deliverResponse(response: ResponseData<T>) = listener.onResponse(response)

    override fun parseNetworkResponse(response: NetworkResponse?): Response<ResponseData<T>> {
        return try {
            val json = String(
                    response?.data ?: ByteArray(0),
                    Charset.forName(HttpHeaderParser.parseCharset(response?.headers)))
            Response.success(
                    ResponseData(response?.statusCode ?: -1, gson.fromJson(json, clazz)),
                    HttpHeaderParser.parseCacheHeaders(response))
        } catch (e: UnsupportedEncodingException) {
            Response.error(ParseError(e))
        } catch (e: JsonSyntaxException) {
            Response.error(ParseError(e))
        }
    }
}
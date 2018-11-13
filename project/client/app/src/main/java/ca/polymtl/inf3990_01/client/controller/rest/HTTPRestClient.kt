package ca.polymtl.inf3990_01.client.controller.rest

import android.content.Context
import android.content.SharedPreferences
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.Volley

/**
 * It's an HTTP client to which we can pass a RESTRequest object.
 */
class HTTPRestClient (private val preferences: SharedPreferences, private val context: Context) {
    private val requestQueue: RequestQueue by lazy {
        // applicationContext is key, it keeps you from leaking the
        // Activity or BroadcastReceiver if someone passes one in.
        Volley.newRequestQueue(context.applicationContext)
    }
    fun <T> addToRequestQueue(req: Request<T>) {
        requestQueue.add(req)
    }

    fun getBaseURL(): String {
        val serverUrl = preferences.getString("server_url", null)
        return "http://$serverUrl"
    }
}
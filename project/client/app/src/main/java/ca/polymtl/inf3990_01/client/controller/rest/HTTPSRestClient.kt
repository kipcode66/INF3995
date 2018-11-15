package ca.polymtl.inf3990_01.client.controller.rest

import android.content.Context
import android.content.SharedPreferences
import com.android.volley.Request
import com.android.volley.RequestQueue
import com.android.volley.toolbox.Volley

/**
 * It's an HTTP client to which we can pass a RESTRequest object.
 */
class HTTPSRestClient (private val preferences: SharedPreferences, private val context: Context) {


    fun <T> addToRequestQueue(req: Request<T>) {

    }
    fun getBaseURL(): String {
            return "true"
    }
}
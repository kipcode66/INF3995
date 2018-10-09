package ca.polymtl.inf3990_01.client.controller.rest

import android.content.SharedPreferences

class HTTPRestClient (val preferences: SharedPreferences) {
    init {
        // We make a first check during the initialization to ensure we can connect to the server.
        var serverUrl = preferences.getString("server_url", null)

    }
}
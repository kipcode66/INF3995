package ca.polymtl.inf3990_01.client.controller.rest

import android.util.Log

class RestRequestService(val tokenMgr: TokenManagerService) {
    fun doRequest(msg: String) = Log.d("App", "[token=${tokenMgr.getToken()}] msg: $msg")
}

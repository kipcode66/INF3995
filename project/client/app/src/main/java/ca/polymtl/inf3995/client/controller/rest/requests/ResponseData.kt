package ca.polymtl.inf3995.client.controller.rest.requests

class ResponseData<T>(
    val code: Int, val value: T, val networkData: com.android.volley.NetworkResponse?
)

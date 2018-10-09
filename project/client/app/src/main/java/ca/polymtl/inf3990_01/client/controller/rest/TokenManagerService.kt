package ca.polymtl.inf3990_01.client.controller.rest

class TokenManagerService(val httpClient: HTTPRestClient) {
    private var token: String? = null
    fun getToken(): String {
        // TODO Ensure that the token is still valid.
        return ""
    }

    private fun fetchToken(): String {
        return ""
    }
}
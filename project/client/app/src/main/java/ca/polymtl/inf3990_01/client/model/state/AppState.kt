package ca.polymtl.inf3990_01.client.model.state

interface AppState {
    val type: AppStateService.State
    fun canDisplayBlackList(): Boolean
    fun canDisplayStatistics(): Boolean
    fun canSendFiles(): Boolean
}
package ca.polymtl.inf3990_01.client.model.state

import java.util.*

class AppStateService: Observable() {
    enum class State {
        User,
        Admin;
    }
    private val states: Map<State, AppState> = mapOf(
            Pair(State.User, object : AppState {
                override val type = State.User
                override fun canDisplayBlackList(): Boolean = false
                override fun canDisplayStatistics(): Boolean = false
                override fun canSendFiles(): Boolean = true
            }),
            Pair(State.Admin, object : AppState {
                override val type = State.Admin
                override fun canDisplayBlackList(): Boolean = true
                override fun canDisplayStatistics(): Boolean = true
                override fun canSendFiles(): Boolean = false
            }))

    private var mStateInternal: AppState = states[State.User]!!

    init {
        setState(State.User)
    }

    fun getState(): AppState {
        return mStateInternal
    }

    fun setState(state: State) {
        mStateInternal = states[state]!!
        setChanged()
        notifyObservers()
    }
}
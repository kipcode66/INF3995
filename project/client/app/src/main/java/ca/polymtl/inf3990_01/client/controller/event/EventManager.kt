package ca.polymtl.inf3990_01.client.controller.event

import android.util.Log
import ca.polymtl.inf3990_01.client.utils.SingletonHolderNoArg

class EventManager private constructor() {
    init {
    }

    fun addEventListener(event: String, callback: ((Event) -> Unit)) {
        Log.d("test", "Hey!")
        return
    }

    enum class EventType {
        CLICK;
    }

    companion object : SingletonHolderNoArg<EventManager>(::EventManager)
}
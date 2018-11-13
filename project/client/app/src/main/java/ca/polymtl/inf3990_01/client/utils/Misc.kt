package ca.polymtl.inf3990_01.client.utils

import java.util.*

class Misc {
    companion object {
        fun toObserver(onUpdate: (Observable, Any?) -> Unit): Observer {
            return Observer(onUpdate)
        }
    }
}
package ca.polymtl.inf3990_01.client.utils

open class SingletonHolderNoArg<out T>(creator: () -> T) {
    private var creator: (() -> T)? = creator
    @Volatile private var instance: T? = null

    fun getInstance(): T {
        val i = instance
        if (i != null) {
            return i
        }

        return synchronized(this) {
            val i2 = instance
            if (i2 != null) {
                i2
            } else {
                val created = creator!!()
                instance = created
                creator = null
                created
            }
        }
    }
}
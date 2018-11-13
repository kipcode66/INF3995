package ca.polymtl.inf3990_01.client.model

open class Song(
    val title:String,
    val authorName:String,
    val durationMS:Int,
    val id:Int,
    val sentBy:String?,
    val ip: String? = null,
    val mac: String? = null,
    val ownerId: Int? = null
)
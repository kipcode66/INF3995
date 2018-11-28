package ca.polymtl.inf3995.client.model

import java.math.BigInteger

open class Song(
    val title: String,
    val authorName: String,
    val durationMS: Int,
    val id: Int,
    val sentBy: String?,
    val ip: String? = null,
    val mac: String? = null,
    val ownerId: BigInteger? = null
)
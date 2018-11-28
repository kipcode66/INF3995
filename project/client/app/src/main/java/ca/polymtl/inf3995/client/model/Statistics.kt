package ca.polymtl.inf3995.client.model

import java.math.BigInteger

data class Statistics(
    val chansons: BigInteger,
    val utilisateurs: BigInteger,
    val elemines: BigInteger,
    val temps: BigInteger
)
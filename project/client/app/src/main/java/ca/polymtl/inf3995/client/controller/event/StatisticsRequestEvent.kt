package ca.polymtl.inf3995.client.controller.event

class StatisticsRequestEvent: Event {
    companion object {
        const val EVENT_TYPE = "statistics_event"
    }

    override val type = EVENT_TYPE
}